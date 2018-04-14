#include <iostream>
#include <stdint.h>

#include <string>

#include <stdio.h>
#include <ostream>
#include <fstream>
#include <type_traits>
#include <thread>

#include <nvtt/nvtt.h>

#include <nvimage/Image.h>    // @@ It might be a good idea to use FreeImage directly instead of ImageIO.
#include <nvimage/ImageIO.h>
#include <nvimage/FloatImage.h>
#include <nvimage/DirectDrawSurface.h>

#include <nvcore/Ptr.h> // AutoPtr
#include <nvcore/StrLib.h> // Path
#include <nvcore/StdStream.h>
#include <nvcore/FileSystem.h>
#include <nvcore/Timer.h>

#include <cmft/image.h>
#include <cmft/cubemapfilter.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "../external/cmft/dependency/stb/stb_image.h"

#define READ_AND_OFFSET(stream, dest, size, offset) stream.read((char*)dest, size); offset += size; stream.seekg(offset);

#define RADIANCE_MAP_SIZE 256
#define CMFT_GLOSS_SCALE 10
#define CMFT_GLOSS_BIAS 3
#define RADIANCE_MAP_MIP_LEVELS 7

namespace TRM
{
    enum AssetType
    {
        ASSET_IMAGE = 0,
        ASSET_AUDIO = 1,
        ASSET_MESH  = 2,
        ASSET_VIDEO = 3
    };
    
    struct FileHeader
    {
        uint32_t magic;
        uint8_t  version;
        uint8_t  type;
    };
    
    namespace V1
    {
        namespace Image
        {
            enum CompressionType
            {
                COMPRESSION_NONE = 0,
                COMPRESSION_BC1  = 1,
                COMPRESSION_BC1a = 2,
                COMPRESSION_BC2  = 3,
                COMPRESSION_BC3  = 4,
                COMPRESSION_BC3n = 5,
                COMPRESSION_BC4 = 6,
                COMPRESSION_BC5 = 7,
                COMPRESSION_BC6 = 8,
                COMPRESSION_BC7 = 9,
                COMPRESSION_ETC1 = 10,
                COMPRESSION_ETC2 = 11,
                COMPRESSION_PVR = 12
            };

			enum PixelType
			{
				PIXEL_TYPE_DEFAULT = 0,
				PIXEL_TYPE_UNORM8 = 8,
				PIXEL_TYPE_FLOAT16 = 16,
				PIXEL_TYPE_FLOAT32 = 32
			};
            
            struct Header
            {
                uint8_t  compression;
                uint8_t  channelSize;
                uint8_t  numChannels;
                uint16_t numArraySlices;
                uint8_t  numMipSlices;
            };
            
            struct MipSliceHeader
            {
                uint16_t width;
                uint16_t height;
				int size;
            };
            
			struct TRMOutputHandler : public nvtt::OutputHandler
			{
				std::fstream* stream;
				long offset;
				int mip_levels = 0;
				int mip_height;
				int compression_type;

				virtual void beginImage(int size, int width, int height, int depth, int face, int miplevel) override
				{
					std::cout << "Beginning Image: Size = " << size << ", Mip = " << miplevel << ", Width = " << width << ", Height = " << height << std::endl;

					MipSliceHeader mip0Header;

					mip0Header.width = width;
					mip0Header.height = height;
					mip0Header.size = size;

					stream->write((char*)&mip0Header, sizeof(MipSliceHeader));
					offset += sizeof(MipSliceHeader);
					stream->seekp(offset);

					mip_height = height;
					mip_levels++;
				}

				// Output data. Compressed data is output as soon as it's generated to minimize memory allocations.
				virtual bool writeData(const void * data, int size) override
				{
					stream->write((char*)data, size);
					offset += size;
					stream->seekp(offset);
					return true;
				}

				// Indicate the end of the compressed image. (New in NVTT 2.1)
				virtual void endImage() override
				{
					std::cout << "Ending Image.." << std::endl;
				}
			};

			template<typename T>
			struct Image
			{
				template<size_t N>
				struct Pixel
				{
					T c[N];
				};

				struct Data
				{
					T* data;
					int width;
					int height;
				};

				Image()
				{
					for (int i = 0; i < 16; i++)
					{
						for (int j = 0; j < 16; j++)
							data[i][j].data = nullptr;
					}
				}

				void unload()
				{
					for (int i = 0; i < array_slices; i++)
					{
						for (int j = 0; j < mip_slices; j++)
						{
							if(data[i][j].data)
								free(data[i][j].data);
						}
					}
				}

				size_t size(int array_slice, int mip_slice) const
				{
					return sizeof(T) * data[array_slice][mip_slice].width * data[array_slice][mip_slice].height * components;
				}

				void to_bgra(int array_slice, int mip_slice)
				{
					Data& imgData = data[array_slice][mip_slice];

					Pixel<4>* src = (Pixel<4>*)imgData.data;

					for (int y = 0; y < imgData.height; y++)
					{
						for (int x = 0; x < imgData.width; x++)
						{
							T red = src[y * imgData.width + x].c[2];

							src[y * imgData.width + x].c[0] = src[y * imgData.width + x].c[2];
							src[y * imgData.width + x].c[2] = red;
						}
					}
				}

				bool to_rgba(Image<T>& img, int array_slice, int mip_slice)
				{
					if (components == 4)
					{
						img = *this;
						return true;
					}

					Data& imgData = data[array_slice][mip_slice];
					Pixel<4>* dst = new Pixel<4>[imgData.width * imgData.height];

					if (components == 3)
					{
						Pixel<3>* src = (Pixel<3>*)imgData.data;

						for (int y = 0; y < imgData.height; y++)
						{
							for (int x = 0; x < imgData.width; x++)
							{
								// Initialize values
								dst[y * imgData.width + x].c[0] = 0;
								dst[y * imgData.width + x].c[1] = 0;
								dst[y * imgData.width + x].c[2] = 0;
								dst[y * imgData.width + x].c[3] = std::numeric_limits<T>::max();

								for (int c = 0; c < components; c++)
								{
									dst[y * imgData.width + x].c[c] = src[y * imgData.width + x].c[c];
								}
							}
						}
					}
					else if (components == 2)
					{
						Pixel<2>* src = (Pixel<2>*)imgData.data;

						for (int y = 0; y < imgData.height; y++)
						{
							for (int x = 0; x < imgData.width; x++)
							{
								// Initialize values
								dst[y * imgData.width + x].c[0] = 0;
								dst[y * imgData.width + x].c[1] = 0;
								dst[y * imgData.width + x].c[2] = 0;
								dst[y * imgData.width + x].c[3] = std::numeric_limits<T>::max();

								for (int c = 0; c < components; c++)
								{
									dst[y * imgData.width + x].c[c] = src[y * imgData.width + x].c[c];
								}
							}
						}
					}
					else if (components == 1)
					{
						Pixel<1>* src = (Pixel<1>*)imgData.data;

						for (int y = 0; y < imgData.height; y++)
						{
							for (int x = 0; x < imgData.width; x++)
							{
								// Initialize values
								dst[y * imgData.width + x].c[0] = 0;
								dst[y * imgData.width + x].c[1] = 0;
								dst[y * imgData.width + x].c[2] = 0;
								dst[y * imgData.width + x].c[3] = std::numeric_limits<T>::max();

								for (int c = 0; c < components; c++)
								{
									dst[y * imgData.width + x].c[c] = src[y * imgData.width + x].c[c];
								}
							}
						}
					}
					
					img.components = 4;
					img.array_slices = array_slices;
					img.mip_slices = mip_slices;
					img.data[array_slice][mip_slice].data = (T*)dst;
					img.data[array_slice][mip_slice].width = data[array_slice][mip_slice].width;
					img.data[array_slice][mip_slice].height = data[array_slice][mip_slice].height;

					return true;
				}

				int components;
				int mip_slices;
				int array_slices;
				Data data[16][16];
			};

			bool image_load(Image<uint8_t>& img, const char* file, int force_cmp = 0) // Used for setting initial mip/array level
			{
				img.data[0][0].data = stbi_load(file, &img.data[0][0].width, &img.data[0][0].height, &img.components, force_cmp);

				if (img.data[0][0].data != nullptr)
				{
					img.array_slices = 1;
					img.mip_slices = 1;

					return true;
				}

				return false;
			}

			bool image_load(Image<float>& img, const char* file, int force_cmp = 0)
			{
				img.data[0][0].data = stbi_loadf(file, &img.data[0][0].width, &img.data[0][0].height, &img.components, force_cmp);

				if (img.data[0][0].data != nullptr)
				{
					img.array_slices = 1;
					img.mip_slices = 1;

					return true;
				}

				return false;
			}

			const char* extension(const char* file)
			{
				int length = strlen(file);

				while (length > 0 && file[length] != '.')
				{
					length--;
				}

				return &file[length + 1];
			}

			const int kChannels[] = {
				1,
				1,
				1,
				2,
				2,
				2,
				3,
				3,
				3,
				4,
				4,
				4
			};

			const nvtt::Format kCompression[] = {
				nvtt::Format_RGB,
				nvtt::Format_BC1,
				nvtt::Format_BC1a,
				nvtt::Format_BC2,
				nvtt::Format_BC3,
				nvtt::Format_BC3n,
				nvtt::Format_BC4,
				nvtt::Format_BC5,
				nvtt::Format_BC6,
				nvtt::Format_BC7
			};

			template<typename T>
			bool export_image(Image<T>& img,
				const char* output,
				PixelType pixel_type = PIXEL_TYPE_DEFAULT,
				CompressionType compression = COMPRESSION_NONE,
				bool normal_map = false,
				int output_mips = 0)
			{
				// Make sure that float images either use no compression or BC6
				if ((std::is_same<T, uint16_t>::value || std::is_same<T, int16_t>::value || std::is_same<T, float>::value) && (compression != COMPRESSION_NONE && compression != COMPRESSION_BC6))
				{
					std::cout << "ERROR::Float images must use either no compression or BC6!" << std::endl;
					return false;
				}

				TRM::FileHeader fh;
				char* magic = (char*)&fh.magic;

				magic[0] = 't';
				magic[1] = 'r';
				magic[2] = 'm';

				fh.version = 1;
				fh.type = TRM::ASSET_IMAGE;

				TRM::V1::Image::Header imageHeader;

				int x = img.data[0][0].width;
				int y = img.data[0][0].height;
				int mX = x;
				int mY = y;

				// output_mips  =  -1 = a full mipchain will be generated.
				// output_mips  =   0 = the existing mips will be used. No new mips will be generated
				// output_mips  >=  1 = N number of miplevels will be generated.
				
				int32_t mipLevels = output_mips;
				bool generate_mipmaps = true;

				if (mipLevels < -1)
				{
					std::cout << "WARNING::mipmaps_to_generate must be greater than or equal to -1. Generating full mipchain..." << std::endl;
					mipLevels = -1;
				}

				if (output_mips == -1)
				{
					mipLevels = 0;

					while (mX > 1 && mY > 1)
					{
						mX = x >> mipLevels;
						mY = y >> mipLevels;

						mipLevels++;
					}

					mipLevels++;
				}
				else if (output_mips == 0)
					mipLevels = img.mip_slices;

				if (mipLevels == 1)
					generate_mipmaps = false;
				
				imageHeader.compression = compression;
				imageHeader.channelSize = sizeof(T);
				imageHeader.numChannels = img.components;
				imageHeader.numArraySlices = img.array_slices;
				imageHeader.numMipSlices = mipLevels;

				const char* file = output; // @TODO: Extract name from filename.
				std::fstream f(output, std::ios::out | std::ios::binary);

				long offset = 0;

				f.seekp(offset);
				f.write((char*)&fh, sizeof(fh));
				offset += sizeof(fh);
				f.seekp(offset);
				uint16_t len = strlen(file);
				f.write((char*)&len, sizeof(uint16_t));
				offset += sizeof(uint16_t);
				f.seekp(offset);
				f.write((char*)file, strlen(file));
				offset += len;
				f.seekp(offset);
				f.write((char*)&imageHeader, sizeof(TRM::V1::Image::Header));
				offset += sizeof(TRM::V1::Image::Header);
				f.seekp(offset);

				TRM::V1::Image::TRMOutputHandler handler;
				nvtt::CompressionOptions compressionOptions;
				nvtt::InputOptions inputOptions;
				nvtt::OutputOptions outputOptions;
				nvtt::Compressor compressor;

				handler.stream = &f;
				handler.offset = offset;

				compressionOptions.setFormat(kCompression[compression]);

				if (compression == COMPRESSION_NONE)
				{
					uint32_t pixel_size = (pixel_type == PIXEL_TYPE_DEFAULT) ? 8 * sizeof(T) : pixel_type;

					if (std::is_same<T, uint8_t>::value)
						compressionOptions.setPixelType(nvtt::PixelType_UnsignedNorm);
					else if (std::is_same<T, uint16_t>::value || std::is_same<T, int16_t>::value || std::is_same<T, float>::value)
						compressionOptions.setPixelType(nvtt::PixelType_Float);

					if (img.components == 4)
						compressionOptions.setPixelFormat(pixel_size, pixel_size, pixel_size, pixel_size);
					else if (img.components == 3)
						compressionOptions.setPixelFormat(pixel_size, pixel_size, pixel_size, 0);
					else if (img.components == 2)
						compressionOptions.setPixelFormat(pixel_size, pixel_size, 0, 0);
					else if (img.components == 1)
						compressionOptions.setPixelFormat(pixel_size, 0, 0, 0);
				}

				if (pixel_type == PIXEL_TYPE_DEFAULT)
				{
					if (std::is_same<T, uint8_t>::value)
						inputOptions.setFormat(nvtt::InputFormat_BGRA_8UB);
					else if (std::is_same<T, uint16_t>::value || std::is_same<T, int16_t>::value)
						inputOptions.setFormat(nvtt::InputFormat_RGBA_16F);
					else if (std::is_same<T, float>::value)
						inputOptions.setFormat(nvtt::InputFormat_RGBA_32F);
				}
				else
				{
					if (pixel_type == PIXEL_TYPE_UNORM8)
						inputOptions.setFormat(nvtt::InputFormat_BGRA_8UB);
					else if (pixel_type == PIXEL_TYPE_FLOAT16)
						inputOptions.setFormat(nvtt::InputFormat_RGBA_16F);
					else if (pixel_type == PIXEL_TYPE_FLOAT32)
						inputOptions.setFormat(nvtt::InputFormat_RGBA_32F);
				}

				inputOptions.setNormalMap(normal_map);
				inputOptions.setConvertToNormalMap(false);
				inputOptions.setNormalizeMipmaps(false);

				outputOptions.setOutputHeader(false);
				outputOptions.setOutputHandler(&handler);

				for (int i = 0; i < img.array_slices; i++)
				{
					Image<T> temp_img;
	
					inputOptions.setTextureLayout(nvtt::TextureType_2D, img.data[i][0].width, img.data[i][0].height);
					
					// If generate_mipmaps is false or if the full mipchain has to be generated, set the data for the initial mip level.
					if (!generate_mipmaps || (generate_mipmaps && output_mips == -1) || (generate_mipmaps && output_mips > 1))  
					{
						img.to_rgba(temp_img, i, 0);
						inputOptions.setMipmapGeneration(generate_mipmaps);
						inputOptions.setMipmapData(temp_img.data[i][0].data, img.data[i][0].width, img.data[i][0].height);
					}
					else if (generate_mipmaps && img.mip_slices > 1)
					{
						inputOptions.setMipmapGeneration(generate_mipmaps, img.mip_slices);

						for (int mip = 0; mip < img.mip_slices; mip++)
						{
							img.to_rgba(temp_img, i, mip);
							inputOptions.setMipmapData(temp_img.data[i][mip].data, img.data[i][mip].width, img.data[i][mip].height, 1, 0, mip);
						}
					}
					else
					{
						temp_img.unload();
						std::cout << "ERROR::Image must contain at least one miplevel" << std::endl;
						return false;
					}

					handler.mip_levels = 0;
					compressor.process(inputOptions, compressionOptions, outputOptions);
					temp_img.unload();
				}

				f.close();

				return true;
			}

			bool export_texture(const char* input,
								const char* output,
								CompressionType compression = COMPRESSION_NONE,
								bool normal_map = false,
								int output_mips = 0)
			{
				std::string ext = extension(input);

				if (ext == "hdr")
				{
					Image<float> img;

					if (!image_load(img, input))
						return false;

					bool result = export_image(img, output, PIXEL_TYPE_DEFAULT, compression, normal_map, output_mips);
					img.unload();

					return result;
				}
				else
				{
					Image<uint8_t> img;

					if (!image_load(img, input))
						return false;

					bool result = export_image(img, output, PIXEL_TYPE_DEFAULT, compression, normal_map, output_mips);
					img.unload();

					return result;
				}
			}

			template<typename T>
			bool cubemap_from_latlong(cmft::Image& dst, Image<T>& src)
			{
				cmft::Image img;
				img.m_width = uint32_t(src.data[0][0].width);
				img.m_height = uint32_t(src.data[0][0].height);
				img.m_dataSize = src.size(0, 0);

				if (src.components < 3)
				{
					std::cout << "ERROR::Image must at least have 3 color channels" << std::endl;
					return false;
				}

				if (src.components == 4)
				{
					if (std::is_same<T, uint8_t>::value)
						img.m_format = cmft::TextureFormat::RGBA8;
					else if (std::is_same<T, uint16_t>::value)
						img.m_format = cmft::TextureFormat::RGBA16F;
					else if (std::is_same<T, float>::value)
						img.m_format = cmft::TextureFormat::RGBA32F;
				}
				else if (src.components == 3)
				{
					if (std::is_same<T, uint8_t>::value)
						img.m_format = cmft::TextureFormat::RGB8;
					else if (std::is_same<T, uint16_t>::value)
						img.m_format = cmft::TextureFormat::RGB16F;
					else if (std::is_same<T, float>::value)
						img.m_format = cmft::TextureFormat::RGB32F;
				}
				
				img.m_numMips = 1;
				img.m_numFaces = 1;
				img.m_data = (void*)src.data[0][0].data;

				if (!cmft::imageIsLatLong(img))
				{
					std::cout << "ERROR::Image is not in latlong format" << std::endl;
					return false;
				}

				if (!cmft::imageCubemapFromLatLong(dst, img))
				{
					std::cout << "ERROR::Failed to convert Cubemap" << std::endl;
					return false;
				}

				return true;
			}

			template<typename T>
			bool cubemap_from_latlong(Image<T>& src,
				const char* name,
				const char* output,
				PixelType pixel_type = PIXEL_TYPE_DEFAULT,
				CompressionType compression = COMPRESSION_NONE,
				bool mipmap = false,
				bool irradiance = false,
				bool radiance = false)
			{
				cmft::Image cmftCube;

				if (!cubemap_from_latlong(cmftCube, src))
				{
					std::cout << "ERROR::Failed to convert Cubemap" << std::endl;
					return false;
				}

				if (irradiance)
				{
					cmft::Image cmftIrradianceCube;

					if (!cmft::imageIrradianceFilterSh(cmftIrradianceCube, 128, cmftCube))
					{
						std::cout << "ERROR::Failed to generate irradiance map!" << std::endl;
						return false;
					}

					Image<T> irradianceCube;

					uint32_t imgOffsets[CUBE_FACE_NUM][MAX_MIP_NUM];
					cmft::imageGetMipOffsets(imgOffsets, cmftIrradianceCube);

					irradianceCube.components = src.components;
					irradianceCube.array_slices = 6;
					irradianceCube.mip_slices = 1;

					for (int i = 0; i < 6; i++)
					{
						uint8_t* mipData = (uint8_t*)cmftIrradianceCube.m_data + imgOffsets[i][0];

						irradianceCube.data[i][0].data = (float*)mipData;
						irradianceCube.data[i][0].height = cmftIrradianceCube.m_height;
						irradianceCube.data[i][0].width = cmftIrradianceCube.m_width;
					}

					std::string out = "irradiance.trm";

					if (!export_image(irradianceCube, out.c_str(), pixel_type, compression, false))
					{
						std::cout << "ERROR::Failed to export Cubemap" << std::endl;
						return false;
					}

					cmft::imageUnload(cmftIrradianceCube);
				}

				if (radiance)
				{
					cmft::Image cmftRadianceCube;

					int threads = std::thread::hardware_concurrency();

					std::cout << "Using " << threads << " threads to generate radiance map" << std::endl;

					if (!cmft::imageRadianceFilter(cmftRadianceCube,
						RADIANCE_MAP_SIZE,
						cmft::LightingModel::BlinnBrdf,
						true,
						RADIANCE_MAP_MIP_LEVELS,
						CMFT_GLOSS_SCALE,
						CMFT_GLOSS_BIAS,
						cmftCube,
						cmft::EdgeFixup::None,
						threads))
					{
						std::cout << "ERROR::Failed to generate radiance map!" << std::endl;
						return false;
					}

					Image<T> radianceCube;

					uint32_t imgOffsets[CUBE_FACE_NUM][MAX_MIP_NUM];
					cmft::imageGetMipOffsets(imgOffsets, cmftRadianceCube);

					radianceCube.components = src.components;
					radianceCube.array_slices = 6;
					radianceCube.mip_slices = 7;

					for (int i = 0; i < 6; i++)
					{
						for (int j = 0; j < 7; j++)
						{
							uint8_t* mipData = (uint8_t*)cmftRadianceCube.m_data + imgOffsets[i][j];

							radianceCube.data[i][j].data = (T*)mipData;
							radianceCube.data[i][j].height = cmftRadianceCube.m_height >> j;
							radianceCube.data[i][j].width = cmftRadianceCube.m_width >> j;
						}
					}

					std::string out = "radiance.trm";

					if (!export_image(radianceCube, out.c_str(), pixel_type, compression, true))
					{
						std::cout << "ERROR::Failed to export Cubemap" << std::endl;
						return false;
					}

					cmft::imageUnload(cmftRadianceCube);
				}

				uint32_t imgOffsets[CUBE_FACE_NUM][MAX_MIP_NUM];
				cmft::imageGetMipOffsets(imgOffsets, cmftCube);

				Image<T> cubemap;

				cubemap.components = src.components;
				cubemap.array_slices = 6;
				cubemap.mip_slices = 1;

				for (int i = 0; i < 6; i++)
				{
					uint8_t* mipData = (uint8_t*)cmftCube.m_data + imgOffsets[i][0];

					cubemap.data[i][0].data = (T*)mipData;
					cubemap.data[i][0].height = cmftCube.m_height;
					cubemap.data[i][0].width = cmftCube.m_width;
				}

				if (!export_image(cubemap, output, pixel_type, compression, mipmap))
				{
					std::cout << "ERROR::Failed to export Cubemap" << std::endl;
					return false;
				}

				cmft::imageUnload(cmftCube);
				src.unload();

				return true;
			}

			bool cubemap_from_latlong(const char* input, 
									  const char* output, 
									  PixelType pixel_type = PIXEL_TYPE_DEFAULT,
									  CompressionType compression = COMPRESSION_NONE,
									  bool mipmap = false,
									  bool irradiance = false,
									  bool radiance = false)
			{
				Image<float> src;

				if (!image_load(src, input))
				{
					std::cout << "ERROR::Failed to open Cubemap" << std::endl;
					return false;
				}

				return cubemap_from_latlong<float>(src, input, output, pixel_type, compression, mipmap, irradiance, radiance);
			}
        };
        
        namespace Audio
        {
            
        }
        
        namespace Video
        {
            
        }
        
        namespace Mesh
        {
            
        }
        
        namespace Animation
        {
            
        }
    }
}

int main(int argc, const char * argv[]) 
{
	std::fstream f("brdfLUT.bin", std::ios::in | std::ios::binary);

	std::streampos begin, end;

	begin = f.tellg();
	f.seekg(0, std::ios::end);
	end = f.tellg();
	f.seekg(0);
	f.seekp(0);

	size_t size = end - begin;

	int16_t* data = new int16_t[512 * 512 * 2];
	f.read((char*)data, size);

	TRM::V1::Image::Image<int16_t> img;

	img.data[0][0].data = data;
	img.data[0][0].width = 512;
	img.data[0][0].height = 512;
	img.components = 2;
	img.array_slices = 1;
	img.mip_slices = 1;

	TRM::V1::Image::export_image(img, "brdfLUT.trm", TRM::V1::Image::PIXEL_TYPE_DEFAULT, TRM::V1::Image::COMPRESSION_NONE, false, 0);

	//TRM::V1::Image::export_texture("C:/Users/Dihara/Pictures/Arches_E_PineTree_3k.hdr",
	//							   "Arches_E_PineTree_3k.trm",
	//							   TRM::V1::Image::FORMAT_R32G32B32,
	//							   TRM::V1::Image::COMPRESSION_NONE,
	//							   true);

	//TRM::V1::Image::export_texture("C:/Users/Dihara/Pictures/Arches_E_PineTree_3k.hdr",
	//							   "Arches_E_PineTree_3k_comp.trm",
	//							   TRM::V1::Image::FORMAT_R16G16B16,
	//							   TRM::V1::Image::COMPRESSION_BC6,
	//							   true);

	//TRM::V1::Image::export_texture("C:/Users/Dihara/Pictures/ak47_by_pabumus.png",
	//	"ak47_by_pabumus.trm",
	//	TRM::V1::Image::FORMAT_DEFAULT,
	//	TRM::V1::Image::COMPRESSION_NONE,
	//	true);

	//TRM::V1::Image::export_texture("C:/Users/Dihara/Pictures/ak47_by_pabumus.jpg",
	//	"ak47_by_pabumus_comp.trm",
	//	TRM::V1::Image::FORMAT_DEFAULT,
	//	TRM::V1::Image::COMPRESSION_BC1,
	//	true);

	//TRM::V1::Image::cubemap_from_latlong("Arches_E_PineTree_3k.hdr", 
	//	"Arches_E_PineTree_3k.trm", 
	//	TRM::V1::Image::PIXEL_TYPE_FLOAT32,
	//	TRM::V1::Image::COMPRESSION_NONE, 
	//	true,
	//	true,
	//	true);

	f.close();

	//{
	//	std::fstream f("brdfLUT.trm", std::ios::in | std::ios::binary);

	//	TRM::FileHeader fileheader;
	//	uint16_t nameLength = 0;
	//	char name[256];
	//	TRM::V1::Image::Header imageHeader;

	//	long offset = 0;

	//	f.seekp(offset);

	//	READ_AND_OFFSET(f, &fileheader, sizeof(TRM::FileHeader), offset);
	//	READ_AND_OFFSET(f, &nameLength, sizeof(uint16_t), offset);
	//	READ_AND_OFFSET(f, &name[0], sizeof(char) * nameLength, offset);

	//	name[nameLength] = '\0';
	//	std::cout << "Name: " << name << std::endl;

	//	READ_AND_OFFSET(f, &imageHeader, sizeof(TRM::V1::Image::Header), offset);

	//	std::cout << "Channel Size: " << imageHeader.channelSize << std::endl;
	//	std::cout << "Channel Count: " << imageHeader.numChannels << std::endl;
	//	std::cout << "Array Slice Count: " << imageHeader.numArraySlices << std::endl;
	//	std::cout << "Mip Slice Count: " << imageHeader.numMipSlices << std::endl;

	//	for (int arraySlice = 0; arraySlice < imageHeader.numArraySlices; arraySlice++)
	//	{
	//		std::cout << std::endl;
	//		std::cout << "Array Slice: " << arraySlice << std::endl;

	//		for (int mipSlice = 0; mipSlice < imageHeader.numMipSlices; mipSlice++)
	//		{
	//			TRM::V1::Image::MipSliceHeader mipHeader;
	//			char* imageData;

	//			READ_AND_OFFSET(f, &mipHeader, sizeof(TRM::V1::Image::MipSliceHeader), offset);

	//			std::cout << std::endl;
	//			std::cout << "Mip Slice: " << mipSlice << std::endl;
	//			std::cout << "Width: " << mipHeader.width << std::endl;
	//			std::cout << "Height: " << mipHeader.height << std::endl;

	//			imageData = (char*)malloc(mipHeader.size);

	//			READ_AND_OFFSET(f, imageData, mipHeader.size, offset);

	//			std::string name = "test_";
	//			name += std::to_string(arraySlice);
	//			name += "_";
	//			name += std::to_string(mipSlice);
	//			name += ".hdr";
	//			//name += ".bmp";

	//			//stbi_write_bmp(name.c_str(), mipHeader.width, mipHeader.height, imageHeader.numChannels, imageData);
	//			stbi_write_hdr(name.c_str(), mipHeader.width, mipHeader.height, imageHeader.numChannels, (float*)imageData);

	//			free(imageData);
	//		}
	//	}
	//}

	delete[] data;

	std::cin.get();
    return 0;
}

