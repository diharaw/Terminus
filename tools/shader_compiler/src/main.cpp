#include <ResourceLimits.h>
#include <Worklist.h>
#include <DirStackFileIncluder.h>
#include <ShHandle.h>
#include <revision.h>
#include <../Public/ShaderLang.h>
#include <GlslangToSpv.h>
#include <GLSL.std.450.h>
#include <doc.h>
#include <disassemble.h>

#include <spirv_glsl.hpp>
#include <spirv_hlsl.hpp>
#include <spirv_msl.hpp>

#include <cstring>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <array>
#include <memory>
#include <thread>

#include <../OSDependent/osinclude.h>

// Command-line options
enum TOptions {
    EOptionNone                 = 0,
    EOptionIntermediate         = (1 <<  0),
    EOptionSuppressInfolog      = (1 <<  1),
    EOptionMemoryLeakMode       = (1 <<  2),
    EOptionRelaxedErrors        = (1 <<  3),
    EOptionGiveWarnings         = (1 <<  4),
    EOptionLinkProgram          = (1 <<  5),
    EOptionMultiThreaded        = (1 <<  6),
    EOptionDumpConfig           = (1 <<  7),
    EOptionDumpReflection       = (1 <<  8),
    EOptionSuppressWarnings     = (1 <<  9),
    EOptionDumpVersions         = (1 << 10),
    EOptionSpv                  = (1 << 11),
    EOptionHumanReadableSpv     = (1 << 12),
    EOptionVulkanRules          = (1 << 13),
    EOptionDefaultDesktop       = (1 << 14),
    EOptionOutputPreprocessed   = (1 << 15),
    EOptionOutputHexadecimal    = (1 << 16),
    EOptionReadHlsl             = (1 << 17),
    EOptionCascadingErrors      = (1 << 18),
    EOptionAutoMapBindings      = (1 << 19),
    EOptionFlattenUniformArrays = (1 << 20),
    EOptionNoStorageFormat      = (1 << 21),
    EOptionKeepUncalled         = (1 << 22),
    EOptionHlslOffsets          = (1 << 23),
    EOptionHlslIoMapping        = (1 << 24),
    EOptionAutoMapLocations     = (1 << 25),
    EOptionDebug                = (1 << 26),
};

//
// Return codes from main/exit().
//
enum TFailCode {
    ESuccess = 0,
    EFailUsage,
    EFailCompile,
    EFailLink,
    EFailCompilerCreate,
    EFailThreadCreate,
    EFailLinkerCreate
};

// Add things like "#define ..." to a preamble to use in the beginning of the shader.
class TPreamble {
public:
    TPreamble() { }
    
    bool isSet() const { return text.size() > 0; }
    const char* get() const { return text.c_str(); }
    
    // #define...
    void addDef(std::string def)
    {
        text.append("#define ");
        fixLine(def);
        
        // The first "=" needs to turn into a space
        int equal = def.find_first_of("=");
        if (equal != def.npos)
            def[equal] = ' ';
        
        text.append(def);
        text.append("\n");
    }
    
    // #undef...
    void addUndef(std::string undef)
    {
        text.append("#undef ");
        fixLine(undef);
        text.append(undef);
        text.append("\n");
    }
    
protected:
    void fixLine(std::string& line)
    {
        // Can't go past a newline in the line
        int end = line.find_first_of("\n");
        if (end != line.npos)
            line = line.substr(0, end);
    }
    
    std::string text;  // contents of preamble
};

// Simple bundling of what makes a compilation unit for ease in passing around,
// and separation of handling file IO versus API (programmatic) compilation.
struct ShaderCompUnit {
    EShLanguage stage;
    static const int maxCount = 1;
    int count;                          // live number of strings/names
    const char* text[maxCount];         // memory owned/managed externally
    std::string fileName[maxCount];     // hold's the memory, but...
    const char* fileNameList[maxCount]; // downstream interface wants pointers
    
    ShaderCompUnit(EShLanguage stage) : stage(stage), count(0) { }
    
    ShaderCompUnit(const ShaderCompUnit& rhs)
    {
        stage = rhs.stage;
        count = rhs.count;
        for (int i = 0; i < count; ++i) {
            fileName[i] = rhs.fileName[i];
            text[i] = rhs.text[i];
            fileNameList[i] = rhs.fileName[i].c_str();
        }
    }
    
    void addString(std::string& ifileName, const char* itext)
    {
        assert(count < maxCount);
        fileName[count] = ifileName;
        text[count] = itext;
        fileNameList[count] = fileName[count].c_str();
        ++count;
    }
};

#if !defined _MSC_VER && !defined MINGW_HAS_SECURE_API

#include <errno.h>

int fopen_s(
            FILE** pFile,
            const char* filename,
            const char* mode
            )
{
    if (!pFile || !filename || !mode) {
        return EINVAL;
    }
    
    FILE* f = fopen(filename, mode);
    if (! f) {
        if (errno != 0) {
            return errno;
        } else {
            return ENOENT;
        }
    }
    *pFile = f;
    
    return 0;
}

#endif

TPreamble UserPreamble;
std::vector<ShaderCompUnit> compUnits;
int Options = 0;
TBuiltInResource Resources;
const char* ExecutableName = nullptr;
const char* binaryFileName = nullptr;
const char* entryPointName = nullptr;
const char* sourceEntryPointName = nullptr;
const char* shaderStageName = nullptr;
const char* variableName = nullptr;
std::vector<std::string> IncludeDirectoryList;
bool CompileFailed = false;
bool LinkFailed = false;
std::array<unsigned int, EShLangCount> baseSamplerBinding;
std::array<unsigned int, EShLangCount> baseTextureBinding;
std::array<unsigned int, EShLangCount> baseImageBinding;
std::array<unsigned int, EShLangCount> baseUboBinding;
std::array<unsigned int, EShLangCount> baseSsboBinding;
std::array<unsigned int, EShLangCount> baseUavBinding;
std::array<std::vector<std::string>, EShLangCount> baseResourceSetBinding;

void PutsIfNonEmpty(const char* str)
{
    if (str && str[0]) {
        puts(str);
    }
}

void StderrIfNonEmpty(const char* str)
{
    if (str && str[0])
        fprintf(stderr, "%s\n", str);
}

void Error(const char* message)
{
    printf("%s: Error %s (use -h for usage)\n", ExecutableName, message);
    exit(EFailUsage);
}

char* ReadFileData(const char* fileName)
{
    FILE *in = nullptr;
    int errorCode = fopen_s(&in, fileName, "r");
    if (errorCode || in == nullptr)
        Error("unable to open input file");
    
    int count = 0;
    while (fgetc(in) != EOF)
        count++;
    
    fseek(in, 0, SEEK_SET);
    
    char* return_data = (char*)malloc(count + 1);  // freed in FreeFileData()
    if ((int)fread(return_data, 1, count, in) != count) {
        free(return_data);
        Error("can't read input file");
    }
    
    return_data[count] = '\0';
    fclose(in);
    
    return return_data;
}

static std::vector<uint32_t> ReadSPIRV(const char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        fprintf(stderr, "Failed to open SPIRV file: %s\n", path);
        return {};
    }
    
    fseek(file, 0, SEEK_END);
    long len = ftell(file) / sizeof(uint32_t);
    rewind(file);
    
    std::vector<uint32_t> spirv(len);
    if (fread(spirv.data(), sizeof(uint32_t), len, file) != size_t(len))
        spirv.clear();
    
    fclose(file);
    return spirv;
}

void SetMessageOptions(EShMessages& messages)
{
    if (Options & EOptionRelaxedErrors)
        messages = (EShMessages)(messages | EShMsgRelaxedErrors);
    if (Options & EOptionIntermediate)
        messages = (EShMessages)(messages | EShMsgAST);
    if (Options & EOptionSuppressWarnings)
        messages = (EShMessages)(messages | EShMsgSuppressWarnings);
    if (Options & EOptionSpv)
        messages = (EShMessages)(messages | EShMsgSpvRules);
    if (Options & EOptionVulkanRules)
        messages = (EShMessages)(messages | EShMsgVulkanRules);
    if (Options & EOptionOutputPreprocessed)
        messages = (EShMessages)(messages | EShMsgOnlyPreprocessor);
    if (Options & EOptionReadHlsl)
        messages = (EShMessages)(messages | EShMsgReadHlsl);
    if (Options & EOptionCascadingErrors)
        messages = (EShMessages)(messages | EShMsgCascadingErrors);
    if (Options & EOptionKeepUncalled)
        messages = (EShMessages)(messages | EShMsgKeepUncalled);
    if (Options & EOptionHlslOffsets)
        messages = (EShMessages)(messages | EShMsgHlslOffsets);
    if (Options & EOptionDebug)
        messages = (EShMessages)(messages | EShMsgDebugInfo);
}

const char* GetBinaryName(EShLanguage stage)
{
    const char* name;
    if (binaryFileName == nullptr) {
        switch (stage) {
            case EShLangVertex:          name = "vert.spv";    break;
            case EShLangTessControl:     name = "tesc.spv";    break;
            case EShLangTessEvaluation:  name = "tese.spv";    break;
            case EShLangGeometry:        name = "geom.spv";    break;
            case EShLangFragment:        name = "frag.spv";    break;
            case EShLangCompute:         name = "comp.spv";    break;
            default:                     name = "unknown";     break;
        }
    } else
        name = binaryFileName;
    
    return name;
}


void CompileAndLinkShaderUnits(std::vector<ShaderCompUnit> compUnits, std::vector<uint32_t>& spirv)
{
    // keep track of what to free
    std::list<glslang::TShader*> shaders;
    
    EShMessages messages = EShMsgDefault;
    SetMessageOptions(messages);
    
    //
    // Per-shader processing...
    //
    
    glslang::TProgram& program = *new glslang::TProgram;
    for (auto it = compUnits.cbegin(); it != compUnits.cend(); ++it) {
        const auto &compUnit = *it;
        glslang::TShader* shader = new glslang::TShader(compUnit.stage);
        shader->setStringsWithLengthsAndNames(compUnit.text, NULL, compUnit.fileNameList, compUnit.count);
        if (entryPointName) // HLSL todo: this needs to be tracked per compUnits
            shader->setEntryPoint(entryPointName);
        if (sourceEntryPointName)
            shader->setSourceEntryPoint(sourceEntryPointName);
        if (UserPreamble.isSet())
            shader->setPreamble(UserPreamble.get());
        
        shader->setShiftSamplerBinding(baseSamplerBinding[compUnit.stage]);
        shader->setShiftTextureBinding(baseTextureBinding[compUnit.stage]);
        shader->setShiftImageBinding(baseImageBinding[compUnit.stage]);
        shader->setShiftUboBinding(baseUboBinding[compUnit.stage]);
        shader->setShiftSsboBinding(baseSsboBinding[compUnit.stage]);
        shader->setShiftUavBinding(baseUavBinding[compUnit.stage]);
        shader->setFlattenUniformArrays((Options & EOptionFlattenUniformArrays) != 0);
        shader->setNoStorageFormat((Options & EOptionNoStorageFormat) != 0);
        shader->setResourceSetBinding(baseResourceSetBinding[compUnit.stage]);
        
        if (Options & EOptionHlslIoMapping)
            shader->setHlslIoMapping(true);
        
        if (Options & EOptionAutoMapBindings)
            shader->setAutoMapBindings(true);
        
        if (Options & EOptionAutoMapLocations)
            shader->setAutoMapLocations(true);
        
        shaders.push_back(shader);
        
        const int defaultVersion = Options & EOptionDefaultDesktop? 110: 100;
        
        DirStackFileIncluder includer;
        std::for_each(IncludeDirectoryList.rbegin(), IncludeDirectoryList.rend(), [&includer](const std::string& dir) {
            includer.pushExternalLocalDirectory(dir); });
        if (Options & EOptionOutputPreprocessed) {
            std::string str;
            if (shader->preprocess(&Resources, defaultVersion, ENoProfile, false, false,
                                   messages, &str, includer)) {
                PutsIfNonEmpty(str.c_str());
            } else {
                CompileFailed = true;
            }
            StderrIfNonEmpty(shader->getInfoLog());
            StderrIfNonEmpty(shader->getInfoDebugLog());
            continue;
        }
        if (! shader->parse(&Resources, defaultVersion, false, messages, includer))
            CompileFailed = true;
        
        program.addShader(shader);
        
        if (! (Options & EOptionSuppressInfolog) &&
            ! (Options & EOptionMemoryLeakMode)) {
            PutsIfNonEmpty(compUnit.fileName[0].c_str());
            PutsIfNonEmpty(shader->getInfoLog());
            PutsIfNonEmpty(shader->getInfoDebugLog());
        }
    }
    
    //
    // Program-level processing...
    //
    
    // Link
    if (! (Options & EOptionOutputPreprocessed) && ! program.link(messages))
        LinkFailed = true;
    
    // Map IO
    if (Options & EOptionSpv) {
        if (!program.mapIO())
            LinkFailed = true;
    }
    
    // Report
    if (! (Options & EOptionSuppressInfolog) &&
        ! (Options & EOptionMemoryLeakMode)) {
        PutsIfNonEmpty(program.getInfoLog());
        PutsIfNonEmpty(program.getInfoDebugLog());
    }
    
    // Reflect
    if (Options & EOptionDumpReflection) {
        program.buildReflection();
        program.dumpReflection();
    }
    
    // Dump SPIR-V
    if (Options & EOptionSpv) {
        if (CompileFailed || LinkFailed)
            printf("SPIR-V is not generated for failed compile or link\n");
        else {
            for (int stage = 0; stage < EShLangCount; ++stage) {
                if (program.getIntermediate((EShLanguage)stage)) {
                    std::string warningsErrors;
                    spv::SpvBuildLogger logger;
                    glslang::SpvOptions spvOptions;
                    if (Options & EOptionDebug)
                        spvOptions.generateDebugInfo = true;
                    glslang::GlslangToSpv(*program.getIntermediate((EShLanguage)stage), spirv, &logger, &spvOptions);
                    
                    // Dump the spv to a file or stdout, etc., but only if not doing
                    // memory/perf testing, as it's not internal to programmatic use.
                    if (! (Options & EOptionMemoryLeakMode)) {
                        printf("%s", logger.getAllMessages().c_str());
                        if (Options & EOptionOutputHexadecimal) {
                            glslang::OutputSpvHex(spirv, GetBinaryName((EShLanguage)stage), variableName);
                        } else {
                            glslang::OutputSpvBin(spirv, GetBinaryName((EShLanguage)stage));
                        }
                        if (Options & EOptionHumanReadableSpv) {
                            spv::Disassemble(std::cout, spirv);
                        }
                    }
                }
            }
        }
    }
    
    // Free everything up, program has to go before the shaders
    // because it might have merged stuff from the shaders, and
    // the stuff from the shaders has to have its destructors called
    // before the pools holding the memory in the shaders is freed.
    delete &program;
    while (shaders.size() > 0) {
        delete shaders.back();
        shaders.pop_back();
    }
}

EShLanguage FindLanguage(const std::string& name, bool parseSuffix=true)
{
    size_t ext = 0;
    std::string suffix;
    
    if (shaderStageName)
        suffix = shaderStageName;
    else {
        // Search for a suffix on a filename: e.g, "myfile.frag".  If given
        // the suffix directly, we skip looking for the '.'
        if (parseSuffix) {
            ext = name.rfind('.');
            if (ext == std::string::npos) {
                return EShLangVertex;
            }
            ++ext;
        }
        suffix = name.substr(ext, std::string::npos);
    }
    
    if (suffix == "vert")
        return EShLangVertex;
    else if (suffix == "tesc")
        return EShLangTessControl;
    else if (suffix == "tese")
        return EShLangTessEvaluation;
    else if (suffix == "geom")
        return EShLangGeometry;
    else if (suffix == "frag")
        return EShLangFragment;
    else if (suffix == "comp")
        return EShLangCompute;
    
    return EShLangVertex;
}

void GenerateGLSL410(std::vector<uint32_t> spirv, std::string shader)
{
    spirv_cross::CompilerGLSL glsl(std::move(spirv));
    glsl.build_combined_image_samplers();
    
    for (auto &remap : glsl.get_combined_image_samplers())
    {
        glsl.set_name(remap.combined_id, glsl.get_name(remap.image_id));
    }
    
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();
    
    for(auto& ubo : resources.uniform_buffers)
    {
        std::string baseType = glsl.get_name(ubo.base_type_id);
        std::string name = baseType.substr(2, baseType.size() - 2);
        
        name[0] = std::tolower(name[0]);
        
        glsl.set_name(ubo.id, name);
    }
    
    spirv_cross::CompilerGLSL::Options options;
    options.version = 410;
    options.es = false;
    options.vulkan_semantics = false;
    options.enable_420pack_extension = false;
    glsl.set_options(options);
    
    std::string source = glsl.compile();
    
    std::ofstream out(shader + "_410.glsl");
    out << source;
    out.close();
}

void GenerateGLSL450(std::vector<uint32_t> spirv, std::string shader)
{
    spirv_cross::CompilerGLSL glsl(std::move(spirv));
    glsl.build_combined_image_samplers();
    
    for (auto &remap : glsl.get_combined_image_samplers())
    {
        glsl.set_name(remap.combined_id, glsl.get_name(remap.image_id));
    }
    
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();
    
    for(auto& ubo : resources.uniform_buffers)
    {
        std::string baseType = glsl.get_name(ubo.base_type_id);
        std::string name = baseType.substr(2, baseType.size() - 2);
        
        name[0] = std::tolower(name[0]);
        
        glsl.set_name(ubo.id, name);
    }
    
    spirv_cross::CompilerGLSL::Options options;
    options.version = 450;
    options.es = false;
    options.vulkan_semantics = false;
    options.enable_420pack_extension = true;
    glsl.set_options(options);
    
    std::string source = glsl.compile();
    
    std::ofstream out(shader + "_450.glsl");
    out << source;
    out.close();
}

void GenerateGLSLVK(std::vector<uint32_t> spirv, std::string shader)
{
    spirv_cross::CompilerGLSL glsl(std::move(spirv));
    
    spirv_cross::CompilerGLSL::Options options;
    options.version = 450;
    options.es = false;
    options.vulkan_semantics = true;
    options.enable_420pack_extension = true;
    glsl.set_options(options);
    
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();
    
    for(auto& ubo : resources.uniform_buffers)
    {
        std::string baseType = glsl.get_name(ubo.base_type_id);
        std::string name = baseType.substr(2, baseType.size() - 2);
        
        name[0] = std::tolower(name[0]);
        
        glsl.set_name(ubo.id, name);
    }
    
    std::string source = glsl.compile();
    
    std::ofstream out(shader + "_vk.glsl");
    out << source;
    out.close();
}

const char* g_TypeTableStr[] = {
    "Unknown",
    "Void",
    "Boolean",
    "Char",
    "Int",
    "UInt",
    "Int64",
    "UInt64",
    "AtomicCounter",
    "Float",
    "Double",
    "Struct",
    "Image",
    "SampledImage",
    "Sampler"
};

size_t g_TypeTableSize[] = {
    0,
    0,
    1,
    1,
    4,
    4,
    8,
    8,
    0,
    4,
    8,
    0,
    0,
    0,
    0
};

void GenerateGLSLES2(std::vector<uint32_t> spirv, std::string shader)
{
    spirv_cross::CompilerGLSL glsl(std::move(spirv));
    glsl.build_combined_image_samplers();
    
    for (auto& remap : glsl.get_combined_image_samplers())
    {
        glsl.set_name(remap.combined_id, glsl.get_name(remap.image_id));
    }
    
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();
    
    for(auto& ubo : resources.uniform_buffers)
    {
        std::string baseType = glsl.get_name(ubo.base_type_id);
        spirv_cross::SPIRType type = glsl.get_type(ubo.base_type_id);
        
        std::cout << baseType << "\n" << std::endl;
        
        for(uint32_t i = 0; i < type.member_types.size(); i++)
        {
            spirv_cross::SPIRType memberType = glsl.get_type(type.member_types[i]);
            std::cout << glsl.get_member_name(ubo.base_type_id, i) << ", Type: " << g_TypeTableStr[memberType.basetype] << ", Size: " << g_TypeTableSize[memberType.basetype];
            
            if(memberType.vecsize > 1 || memberType.columns > 1)
                std::cout << ", Columns: " << memberType.columns << ", Rows: " << memberType.vecsize;
            
            if(memberType.array.size() > 0)
                std::cout << ", Array Size: " << memberType.array[0] << ", Total Size: " << g_TypeTableSize[memberType.basetype] * memberType.array[0] << std::endl;
            else
                std::cout << std::endl;
        }
        
        std::cout << "\n" << std::endl;
        
        std::string name = baseType.substr(2, baseType.size() - 2);
        
        name[0] = std::tolower(name[0]);
        
        glsl.set_name(ubo.id, name);
    }
    
    spirv_cross::CompilerGLSL::Options options;
    options.version = 200;
    options.es = true;
    options.vulkan_semantics = false;
    options.enable_420pack_extension = true;
    glsl.set_options(options);
    
    std::string source = glsl.compile();
    
    std::ofstream out(shader + "_es2.glsl");
    out << source;
    out.close();
}

void GenerateGLSLES3(std::vector<uint32_t> spirv, std::string shader)
{
    spirv_cross::CompilerGLSL glsl(std::move(spirv));
    glsl.build_combined_image_samplers();
    
    for (auto &remap : glsl.get_combined_image_samplers())
    {
        glsl.set_name(remap.combined_id, glsl.get_name(remap.image_id));
    }
    
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();
    
    for(auto& ubo : resources.uniform_buffers)
    {
        std::string baseType = glsl.get_name(ubo.base_type_id);
        std::string name = baseType.substr(2, baseType.size() - 2);
        
        name[0] = std::tolower(name[0]);
        
        glsl.set_name(ubo.id, name);
    }
    
    spirv_cross::CompilerGLSL::Options options;
    options.version = 300;
    options.es = true;
    options.vulkan_semantics = false;
    options.enable_420pack_extension = true;
    glsl.set_options(options);
    
    std::string source = glsl.compile();
    
    std::ofstream out(shader + "_es3.glsl");
    out << source;
    out.close();
}

void GenerateMSL(std::vector<uint32_t> spirv, std::string shader)
{
    spirv_cross::CompilerMSL msl(std::move(spirv));
    
    spirv_cross::CompilerMSL::Options options;
    
    std::string source = msl.compile();
    
    std::ofstream out(shader + ".metal");
    out << source;
    out.close();
}

struct ShaderBinary
{

	size_t binarySize;
	const char* binary;
};

bool CrossCompileHLSL(const std::vector<std::string>& shaders, const std::string& entryPoint, const std::vector<std::string>& defines)
{
    Options |= EOptionReadHlsl;
    Options |= EOptionSpv;
    Options |= EOptionVulkanRules;
    Options |= EOptionLinkProgram;
    
    for(auto define : defines)
        UserPreamble.addDef(define);
    
    glslang::InitializeProcess();
    
    for(auto shader : shaders)
    {
        std::vector<ShaderCompUnit> compUnits;
        std::vector<uint32_t> spirv;
        
        std::string filename = shader;
        entryPointName = entryPoint.c_str();
        
        ShaderCompUnit compUnit(FindLanguage(filename));
        char* fileText = ReadFileData(filename.c_str());
        
        if (fileText == nullptr)
        {
            Error("Failed to read shader source.");
            return false;
        }
        
        compUnit.addString(filename, fileText);
        compUnits.push_back(compUnit);
        
        CompileAndLinkShaderUnits(compUnits, spirv);
        
        GenerateGLSL410(spirv, shader);
        GenerateGLSL450(spirv, shader);
        GenerateGLSLVK(spirv, shader);
        GenerateGLSLES2(spirv, shader);
        GenerateGLSLES3(spirv, shader);
//        GenerateMSL(spirv, shader);
        
//        spirv_cross::ShaderResources resources = glsl.get_shader_resources();
//
//        printf("\n\n******* Resources for shader: %s *******\n\n", shader.c_str());
//
//        for (auto &resource : resources.storage_buffers)
//        {
//            unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
//            unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
//            printf("Storage Buffer %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);
//        }
//
//        printf("\n");
//
//        for (auto &resource : resources.uniform_buffers)
//        {
//            unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
//            unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
//            printf("Uniform Buffer %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);
//        }
//
//        printf("\n");
//
//        for (auto &resource : resources.separate_images)
//        {
//            unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
//            unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
//            printf("Separate Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);
//        }
//
//        printf("\n");
//
//        for (auto &resource : resources.separate_samplers)
//        {
//            unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
//            unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
//            printf("Separate Sampler %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);
//        }
//
//        spirv_cross::CompilerGLSL::Options options;
//        options.version = 450;
//        options.es = false;
//        options.vulkan_semantics = true;
//        options.enable_420pack_extension = true;
//        glsl.set_options(options);
//
//        std::string source = glsl.compile();
//        std::cout << source << std::endl;
        
        free(fileText);
    }
    
    glslang::FinalizeProcess();
    
    return true;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> shaders;
    std::vector<std::string> defines;
    
    defines.push_back("USE_DIFFUSE_MAP");
    
    shaders.push_back("ps_test_template.frag");
    shaders.push_back("vs_test_template.vert");
    
    std::string entry = "TEShaderEntry";
    
    CrossCompileHLSL(shaders, entry, defines);

	std::cin.get();
    
    return 0;
}
