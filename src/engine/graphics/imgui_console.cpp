#include <Graphics/imgui_console.h>
#include <iostream>

#ifdef _WIN32
#define IM_NEWLINE "\r\n"
#else
#define IM_NEWLINE "\n"
#endif

#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
#define IM_MAX(_A,_B)       (((_A) >= (_B)) ? (_A) : (_B))

namespace terminus
{
    namespace imgui_console
    {
        static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
        static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
        static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }
        
        struct ImGuiConsoleState
        {
            char                  InputBuf[256];
            ImVector<char*>       Items;
            bool                  ScrollToBottom;
            ImVector<char*>       History;
            int                   HistoryPos;
            ImVector<const char*> Commands;
            bool				  m_Opened;
        };
        
        ImGuiConsoleState g_imgui_console;
        
        void initialize()
        {
            clear_log();
            memset(g_imgui_console.InputBuf, 0, sizeof(g_imgui_console.InputBuf));
            g_imgui_console.HistoryPos = -1;
            g_imgui_console.m_Opened = true;
        }
        
        void shutdown()
        {
            clear_log();
            for (int i = 0; i < g_imgui_console.History.Size; i++)
                free(g_imgui_console.History[i]);
        }
        
        void logger_callback(std::string text, LogLevel level)
        {
            log(text.c_str());
        }
        
        void log(const char* fmt, ...)
        {
            char buf[1024];
            va_list args;
            va_start(args, fmt);
            vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
            buf[IM_ARRAYSIZE(buf) - 1] = 0;
            va_end(args);
            g_imgui_console.Items.push_back(Strdup(buf));
            g_imgui_console.ScrollToBottom = true;
        }
        
        void clear_log()
        {
            for (int i = 0; i < g_imgui_console.Items.Size; i++)
                free(g_imgui_console.Items[i]);
            g_imgui_console.Items.clear();
            g_imgui_console.ScrollToBottom = true;
        }
        
        void exec_command(const char* command_line)
        {
            log("# %s\n", command_line);
            
            // Insert into history. First find match and delete it so it can be pushed to the back. This isn't trying to be smart or optimal.
            g_imgui_console.HistoryPos = -1;
            for (int i = g_imgui_console.History.Size - 1; i >= 0; i--)
                if (Stricmp(g_imgui_console.History[i], command_line) == 0)
                {
                    free(g_imgui_console.History[i]);
                    g_imgui_console.History.erase(g_imgui_console.History.begin() + i);
                    break;
                }
            g_imgui_console.History.push_back(Strdup(command_line));
            
            // Process command
            if (Stricmp(command_line, "CLEAR") == 0)
            {
                clear_log();
            }
            else if (Stricmp(command_line, "HELP") == 0)
            {
                log("Commands:");
                for (int i = 0; i < g_imgui_console.Commands.Size; i++)
                    log("- %s", g_imgui_console.Commands[i]);
            }
            else if (Stricmp(command_line, "HISTORY") == 0)
            {
                for (int i = g_imgui_console.History.Size >= 10 ? g_imgui_console.History.Size - 10 : 0; i < g_imgui_console.History.Size; i++)
                    log("%3d: %s\n", i, g_imgui_console.History[i]);
            }
            else
            {
                log("Unknown command: '%s'\n", command_line);
            }
        }
        
        int text_edit_callback(ImGuiTextEditCallbackData* data)
        {
            return text_edit_callback(data);
        }
        
        int text_edit_callback_stub(ImGuiTextEditCallbackData* data)
        {
            switch (data->EventFlag)
            {
                case ImGuiInputTextFlags_CallbackCompletion:
                {
                    // Example of TEXT COMPLETION
                    
                    // Locate beginning of current word
                    const char* word_end = data->Buf + data->CursorPos;
                    const char* word_start = word_end;
                    while (word_start > data->Buf)
                    {
                        const char c = word_start[-1];
                        if (c == ' ' || c == '\t' || c == ',' || c == ';')
                            break;
                        word_start--;
                    }
                    
                    // Build a list of candidates
                    ImVector<const char*> candidates;
                    for (int i = 0; i < g_imgui_console.Commands.Size; i++)
                        if (Strnicmp(g_imgui_console.Commands[i], word_start, (int)(word_end - word_start)) == 0)
                            candidates.push_back(g_imgui_console.Commands[i]);
                    
                    if (candidates.Size == 0)
                    {
                        // No match
                        log("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
                    }
                    else if (candidates.Size == 1)
                    {
                        // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing
                        data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                        data->InsertChars(data->CursorPos, candidates[0]);
                        data->InsertChars(data->CursorPos, " ");
                    }
                    else
                    {
                        // Multiple matches. Complete as much as we can, so inputing "C" will complete to "CL" and display "CLEAR" and "CLASSIFY"
                        int match_len = (int)(word_end - word_start);
                        for (;;)
                        {
                            int c = 0;
                            bool all_candidates_matches = true;
                            for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                                if (i == 0)
                                    c = toupper(candidates[i][match_len]);
                                else if (c != toupper(candidates[i][match_len]))
                                    all_candidates_matches = false;
                            if (!all_candidates_matches)
                                break;
                            match_len++;
                        }
                        
                        if (match_len > 0)
                        {
                            data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                            data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                        }
                        
                        // List matches
                        log("Possible matches:\n");
                        for (int i = 0; i < candidates.Size; i++)
                            log("- %s\n", candidates[i]);
                    }
                    
                    break;
                }
                case ImGuiInputTextFlags_CallbackHistory:
                {
                    // Example of HISTORY
                    const int prev_history_pos = g_imgui_console.HistoryPos;
                    if (data->EventKey == ImGuiKey_UpArrow)
                    {
                        if (g_imgui_console.HistoryPos == -1)
                            g_imgui_console.HistoryPos = g_imgui_console.History.Size - 1;
                        else if (g_imgui_console.HistoryPos > 0)
                            g_imgui_console.HistoryPos--;
                    }
                    else if (data->EventKey == ImGuiKey_DownArrow)
                    {
                        if (g_imgui_console.HistoryPos != -1)
                            if (++g_imgui_console.HistoryPos >= g_imgui_console.History.Size)
                                g_imgui_console.HistoryPos = -1;
                    }
                    
                    // A better implementation would preserve the data on the current input line along with cursor position.
                    if (prev_history_pos != g_imgui_console.HistoryPos)
                    {
                        data->CursorPos = data->SelectionStart = data->SelectionEnd = data->BufTextLen = (int)snprintf(data->Buf, (size_t)data->BufSize, "%s", (g_imgui_console.HistoryPos >= 0) ? g_imgui_console.History[g_imgui_console.HistoryPos] : "");
                        data->BufDirty = true;
                    }
                }
            }
            
            return 0;
        }
        
        void draw()
        {
            if (ImGui::SmallButton("Clear")) clear_log(); ImGui::SameLine();
            if (ImGui::SmallButton("Scroll to bottom")) g_imgui_console.ScrollToBottom = true;
            //static float t = 0.0f; if (ImGui::GetTime() - t > 0.02f) { t = ImGui::GetTime(); AddLog("Spam %f", t); }
            
            ImGui::Separator();
            
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
            static ImGuiTextFilter filter;
            filter.Draw("Filter", 180);
            ImGui::PopStyleVar();
            ImGui::Separator();
            
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()), false, ImGuiWindowFlags_HorizontalScrollbar);
            if (ImGui::BeginPopupContextWindow())
            {
                if (ImGui::Selectable("Clear")) clear_log();
                ImGui::EndPopup();
            }
            
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
            for (int i = 0; i < g_imgui_console.Items.Size; i++)
            {
                const char* item = g_imgui_console.Items[i];
                if (!filter.PassFilter(item))
                    continue;
                ImVec4 col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // A better implementation may store a type per-item. For the sample let's just parse the text.
                
                if (strstr(item, "[ ERROR ]"))
                    col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
                else if (strncmp(item, "# ", 2) == 0)
                    col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
                else if (strstr(item, "[ WARN ]"))
                    col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
                
                ImGui::PushStyleColor(ImGuiCol_Text, col);
                ImGui::TextUnformatted(item);
                ImGui::PopStyleColor();
            }
            if (g_imgui_console.ScrollToBottom)
                ImGui::SetScrollHere();
            g_imgui_console.ScrollToBottom = false;
            ImGui::PopStyleVar();
            ImGui::EndChild();
            ImGui::Separator();
            
            // Command-line
            if (ImGui::InputText("Input", g_imgui_console.InputBuf, IM_ARRAYSIZE(g_imgui_console.InputBuf), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory, &text_edit_callback_stub, nullptr))
            {
                char* input_end = g_imgui_console.InputBuf + strlen(g_imgui_console.InputBuf);
                while (input_end > g_imgui_console.InputBuf && input_end[-1] == ' ') input_end--; *input_end = 0;
                if (g_imgui_console.InputBuf[0])
                    exec_command(g_imgui_console.InputBuf);
                strcpy(g_imgui_console.InputBuf, "");
            }
            
            // Demonstrate keeping auto focus on the input box
            if (ImGui::IsItemHovered() || (ImGui::IsRootWindowOrAnyChildFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0)))
                ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
        }
    }
}
