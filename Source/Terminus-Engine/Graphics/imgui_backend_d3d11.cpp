#include <Core/config.h>

#if defined(TERMINUS_DIRECT3D11)

#include <Graphics/imgui_backend.h>
#include <IO/filesystem.h>
#include <Core/context.h>

namespace terminus { namespace ImGuiBackend {

	static SDL_Window*  g_Window = NULL;
	static double       g_Time = 0.0f;
	static bool         g_MousePressed[3] = { false, false, false };
	static float        g_MouseWheel = 0.0f;
	static HWND                      g_hWnd = 0;
	static ID3D11Device*             g_pd3dDevice = NULL;
	static ID3D11DeviceContext*      g_pd3dDeviceContext = NULL;
	static ID3D11Buffer*             g_pVB = NULL;
	static ID3D11Buffer*             g_pIB = NULL;
	static ID3D10Blob *              g_pVertexShaderBlob = NULL;
	static ID3D11VertexShader*       g_pVertexShader = NULL;
	static ID3D11InputLayout*        g_pInputLayout = NULL;
	static ID3D11Buffer*             g_pVertexConstantBuffer = NULL;
	static ID3D10Blob *              g_pPixelShaderBlob = NULL;
	static ID3D11PixelShader*        g_pPixelShader = NULL;
	static ID3D11SamplerState*       g_pFontSampler = NULL;
	static ID3D11ShaderResourceView* g_pFontTextureView = NULL;
	static ID3D11RasterizerState*    g_pRasterizerState = NULL;
	static ID3D11BlendState*         g_pBlendState = NULL;
	static ID3D11DepthStencilState*  g_pDepthStencilState = NULL;
	static int                       g_VertexBufferSize = 5000, g_IndexBufferSize = 10000;

	struct VERTEX_CONSTANT_BUFFER
	{
		float        mvp[4][4];
	};

	static const char* get_clipboard_text()
	{
		return SDL_GetClipboardText();
	}

	static void set_clipboard_text(const char* text)
	{
		SDL_SetClipboardText(text);
	}

	bool process_window_events(SDL_Event* event)
	{
		ImGuiIO& io = ImGui::GetIO();
		switch (event->type)
		{
		case SDL_MOUSEWHEEL:
		{
			if (event->wheel.y > 0)
				g_MouseWheel = 1;
			if (event->wheel.y < 0)
				g_MouseWheel = -1;
			return true;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			if (event->button.button == SDL_BUTTON_LEFT) g_MousePressed[0] = true;
			if (event->button.button == SDL_BUTTON_RIGHT) g_MousePressed[1] = true;
			if (event->button.button == SDL_BUTTON_MIDDLE) g_MousePressed[2] = true;
			return true;
		}
		case SDL_TEXTINPUT:
		{
			io.AddInputCharactersUTF8(event->text.text);
			return true;
		}
		case SDL_KEYDOWN:
		case SDL_KEYUP:
		{
			int key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
			io.KeysDown[key] = (event->type == SDL_KEYDOWN);
			io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
			io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
			io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
			io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
			return true;
		}
		}
		return false;
	}

	void render_callback(ImDrawData* draw_data)
	{
		ID3D11DeviceContext* ctx = g_pd3dDeviceContext;

		if (!g_pVB || g_VertexBufferSize < draw_data->TotalVtxCount)
		{
			if (g_pVB) { g_pVB->Release(); g_pVB = NULL; }
			g_VertexBufferSize = draw_data->TotalVtxCount + 5000;
			D3D11_BUFFER_DESC desc;
			memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = g_VertexBufferSize * sizeof(ImDrawVert);
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			desc.MiscFlags = 0;
			if (g_pd3dDevice->CreateBuffer(&desc, NULL, &g_pVB) < 0)
				return;
		}
		if (!g_pIB || g_IndexBufferSize < draw_data->TotalIdxCount)
		{
			if (g_pIB) { g_pIB->Release(); g_pIB = NULL; }
			g_IndexBufferSize = draw_data->TotalIdxCount + 10000;
			D3D11_BUFFER_DESC desc;
			memset(&desc, 0, sizeof(D3D11_BUFFER_DESC));
			desc.Usage = D3D11_USAGE_DYNAMIC;
			desc.ByteWidth = g_IndexBufferSize * sizeof(ImDrawIdx);
			desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			if (g_pd3dDevice->CreateBuffer(&desc, NULL, &g_pIB) < 0)
				return;
		}

		// Copy and convert all vertices into a single contiguous buffer
		D3D11_MAPPED_SUBRESOURCE vtx_resource, idx_resource;
		if (ctx->Map(g_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &vtx_resource) != S_OK)
			return;
		if (ctx->Map(g_pIB, 0, D3D11_MAP_WRITE_DISCARD, 0, &idx_resource) != S_OK)
			return;
		ImDrawVert* vtx_dst = (ImDrawVert*)vtx_resource.pData;
		ImDrawIdx* idx_dst = (ImDrawIdx*)idx_resource.pData;
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			memcpy(vtx_dst, &cmd_list->VtxBuffer[0], cmd_list->VtxBuffer.size() * sizeof(ImDrawVert));
			memcpy(idx_dst, &cmd_list->IdxBuffer[0], cmd_list->IdxBuffer.size() * sizeof(ImDrawIdx));
			vtx_dst += cmd_list->VtxBuffer.size();
			idx_dst += cmd_list->IdxBuffer.size();
		}
		ctx->Unmap(g_pVB, 0);
		ctx->Unmap(g_pIB, 0);

		// Setup orthographic projection matrix into our constant buffer
		{
			D3D11_MAPPED_SUBRESOURCE mapped_resource;
			if (ctx->Map(g_pVertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource) != S_OK)
				return;
			VERTEX_CONSTANT_BUFFER* constant_buffer = (VERTEX_CONSTANT_BUFFER*)mapped_resource.pData;
			float L = 0.0f;
			float R = ImGui::GetIO().DisplaySize.x;
			float B = ImGui::GetIO().DisplaySize.y;
			float T = 0.0f;
			float mvp[4][4] =
			{
				{ 2.0f / (R - L),   0.0f,           0.0f,       0.0f },
				{ 0.0f,         2.0f / (T - B),     0.0f,       0.0f },
				{ 0.0f,         0.0f,           0.5f,       0.0f },
				{ (R + L) / (L - R),  (T + B) / (B - T),    0.5f,       1.0f },
			};
			memcpy(&constant_buffer->mvp, mvp, sizeof(mvp));
			ctx->Unmap(g_pVertexConstantBuffer, 0);
		}

		// Backup DX state that will be modified to restore it afterwards (unfortunately this is very ugly looking and verbose. Close your eyes!)
		struct BACKUP_DX11_STATE
		{
			UINT                        ScissorRectsCount, ViewportsCount;
			D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
			D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
			ID3D11RasterizerState*      RS;
			ID3D11BlendState*           BlendState;
			FLOAT                       BlendFactor[4];
			UINT                        SampleMask;
			UINT                        StencilRef;
			ID3D11DepthStencilState*    DepthStencilState;
			ID3D11ShaderResourceView*   PSShaderResource;
			ID3D11SamplerState*         PSSampler;
			ID3D11PixelShader*          PS;
			ID3D11VertexShader*         VS;
			UINT                        PSInstancesCount, VSInstancesCount;
			ID3D11ClassInstance*        PSInstances[256], *VSInstances[256];   // 256 is max according to PSSetShader documentation
			D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
			ID3D11Buffer*               IndexBuffer, *VertexBuffer, *VSConstantBuffer;
			UINT                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
			DXGI_FORMAT                 IndexBufferFormat;
			ID3D11InputLayout*          InputLayout;
		};
		BACKUP_DX11_STATE old;
		old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
		ctx->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
		ctx->RSGetViewports(&old.ViewportsCount, old.Viewports);
		ctx->RSGetState(&old.RS);
		ctx->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
		ctx->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
		ctx->PSGetShaderResources(0, 1, &old.PSShaderResource);
		ctx->PSGetSamplers(0, 1, &old.PSSampler);
		old.PSInstancesCount = old.VSInstancesCount = 256;
		ctx->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
		ctx->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
		ctx->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
		ctx->IAGetPrimitiveTopology(&old.PrimitiveTopology);
		ctx->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
		ctx->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
		ctx->IAGetInputLayout(&old.InputLayout);

		// Setup viewport
		D3D11_VIEWPORT vp;
		memset(&vp, 0, sizeof(D3D11_VIEWPORT));
		vp.Width = ImGui::GetIO().DisplaySize.x;
		vp.Height = ImGui::GetIO().DisplaySize.y;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = vp.TopLeftY = 0.0f;
		ctx->RSSetViewports(1, &vp);

		// Bind shader and vertex buffers
		unsigned int stride = sizeof(ImDrawVert);
		unsigned int offset = 0;
		ctx->IASetInputLayout(g_pInputLayout);
		ctx->IASetVertexBuffers(0, 1, &g_pVB, &stride, &offset);
		ctx->IASetIndexBuffer(g_pIB, sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT, 0);
		ctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		ctx->VSSetShader(g_pVertexShader, NULL, 0);
		ctx->VSSetConstantBuffers(0, 1, &g_pVertexConstantBuffer);
		ctx->PSSetShader(g_pPixelShader, NULL, 0);
		ctx->PSSetSamplers(0, 1, &g_pFontSampler);

		// Setup render state
		const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
		ctx->OMSetBlendState(g_pBlendState, blend_factor, 0xffffffff);
		ctx->OMSetDepthStencilState(g_pDepthStencilState, 0);
		ctx->RSSetState(g_pRasterizerState);

		// Render command lists
		int vtx_offset = 0;
		int idx_offset = 0;
		for (int n = 0; n < draw_data->CmdListsCount; n++)
		{
			const ImDrawList* cmd_list = draw_data->CmdLists[n];
			for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.size(); cmd_i++)
			{
				const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
				if (pcmd->UserCallback)
				{
					pcmd->UserCallback(cmd_list, pcmd);
				}
				else
				{
					const D3D11_RECT r = { (LONG)pcmd->ClipRect.x, (LONG)pcmd->ClipRect.y, (LONG)pcmd->ClipRect.z, (LONG)pcmd->ClipRect.w };
					ctx->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&pcmd->TextureId);
					ctx->RSSetScissorRects(1, &r);
					ctx->DrawIndexed(pcmd->ElemCount, idx_offset, vtx_offset);
				}
				idx_offset += pcmd->ElemCount;
			}
			vtx_offset += cmd_list->VtxBuffer.size();
		}

		// Restore modified DX state
		ctx->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
		ctx->RSSetViewports(old.ViewportsCount, old.Viewports);
		ctx->RSSetState(old.RS); if (old.RS) old.RS->Release();
		ctx->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
		ctx->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
		ctx->PSSetShaderResources(0, 1, &old.PSShaderResource); if (old.PSShaderResource) old.PSShaderResource->Release();
		ctx->PSSetSamplers(0, 1, &old.PSSampler); if (old.PSSampler) old.PSSampler->Release();
		ctx->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
		for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
		ctx->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
		ctx->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
		for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
		ctx->IASetPrimitiveTopology(old.PrimitiveTopology);
		ctx->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
		ctx->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
		ctx->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();
	}

	bool create_fonts_texture()
	{
		// Build texture atlas
		ImGuiIO& io = ImGui::GetIO();
		unsigned char* pixels;
		int width, height;
		io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

		// Upload texture to graphics system
		{
			D3D11_TEXTURE2D_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;

			ID3D11Texture2D *pTexture = NULL;
			D3D11_SUBRESOURCE_DATA subResource;
			subResource.pSysMem = pixels;
			subResource.SysMemPitch = desc.Width * 4;
			subResource.SysMemSlicePitch = 0;
			g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);

			// Create texture view
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			ZeroMemory(&srvDesc, sizeof(srvDesc));
			srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MipLevels = desc.MipLevels;
			srvDesc.Texture2D.MostDetailedMip = 0;
			g_pd3dDevice->CreateShaderResourceView(pTexture, &srvDesc, &g_pFontTextureView);
			pTexture->Release();
		}

		// Store our identifier
		io.Fonts->TexID = (void *)g_pFontTextureView;

		// Create texture sampler
		{
			D3D11_SAMPLER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			desc.MipLODBias = 0.f;
			desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
			desc.MinLOD = 0.f;
			desc.MaxLOD = 0.f;
			g_pd3dDevice->CreateSamplerState(&desc, &g_pFontSampler);
		}

		return true;
	}

	void InvalidateDeviceObjects()
	{
		if (!g_pd3dDevice)
			return;

		if (g_pFontSampler) { g_pFontSampler->Release(); g_pFontSampler = NULL; }
		if (g_pFontTextureView) { g_pFontTextureView->Release(); g_pFontTextureView = NULL; ImGui::GetIO().Fonts->TexID = 0; }
		if (g_pIB) { g_pIB->Release(); g_pIB = NULL; }
		if (g_pVB) { g_pVB->Release(); g_pVB = NULL; }

		if (g_pBlendState) { g_pBlendState->Release(); g_pBlendState = NULL; }
		if (g_pDepthStencilState) { g_pDepthStencilState->Release(); g_pDepthStencilState = NULL; }
		if (g_pRasterizerState) { g_pRasterizerState->Release(); g_pRasterizerState = NULL; }
		if (g_pPixelShader) { g_pPixelShader->Release(); g_pPixelShader = NULL; }
		if (g_pPixelShaderBlob) { g_pPixelShaderBlob->Release(); g_pPixelShaderBlob = NULL; }
		if (g_pVertexConstantBuffer) { g_pVertexConstantBuffer->Release(); g_pVertexConstantBuffer = NULL; }
		if (g_pInputLayout) { g_pInputLayout->Release(); g_pInputLayout = NULL; }
		if (g_pVertexShader) { g_pVertexShader->Release(); g_pVertexShader = NULL; }
		if (g_pVertexShaderBlob) { g_pVertexShaderBlob->Release(); g_pVertexShaderBlob = NULL; }
	}

	bool create_device_objects()
	{
		if (g_pFontSampler)
			InvalidateDeviceObjects();

		{
			static const char* vertexShader =
				"cbuffer vertexBuffer : register(b0) \
            {\
            float4x4 ProjectionMatrix; \
            };\
            struct VS_INPUT\
            {\
            float2 pos : POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            \
            struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            \
            PS_INPUT main(VS_INPUT input)\
            {\
            PS_INPUT output;\
            output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
            output.col = input.col;\
            output.uv  = input.uv;\
            return output;\
            }";

			D3DCompile(vertexShader, strlen(vertexShader), NULL, NULL, NULL, "main", "vs_4_0", 0, 0, &g_pVertexShaderBlob, NULL);
			if (g_pVertexShaderBlob == NULL) // NB: Pass ID3D10Blob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
				return false;
			if (g_pd3dDevice->CreateVertexShader((DWORD*)g_pVertexShaderBlob->GetBufferPointer(), g_pVertexShaderBlob->GetBufferSize(), NULL, &g_pVertexShader) != S_OK)
				return false;

			// Create the input layout
			D3D11_INPUT_ELEMENT_DESC local_layout[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->pos), D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (size_t)(&((ImDrawVert*)0)->uv),  D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (size_t)(&((ImDrawVert*)0)->col), D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			if (g_pd3dDevice->CreateInputLayout(local_layout, 3, g_pVertexShaderBlob->GetBufferPointer(), g_pVertexShaderBlob->GetBufferSize(), &g_pInputLayout) != S_OK)
				return false;

			// Create the constant buffer
			{
				D3D11_BUFFER_DESC desc;
				desc.ByteWidth = sizeof(VERTEX_CONSTANT_BUFFER);
				desc.Usage = D3D11_USAGE_DYNAMIC;
				desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
				desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				desc.MiscFlags = 0;
				g_pd3dDevice->CreateBuffer(&desc, NULL, &g_pVertexConstantBuffer);
			}
		}

		// Create the pixel shader
		{
			static const char* pixelShader =
				"struct PS_INPUT\
            {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
            };\
            sampler sampler0;\
            Texture2D texture0;\
            \
            float4 main(PS_INPUT input) : SV_Target\
            {\
            float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
            return out_col; \
            }";

			D3DCompile(pixelShader, strlen(pixelShader), NULL, NULL, NULL, "main", "ps_4_0", 0, 0, &g_pPixelShaderBlob, NULL);
			if (g_pPixelShaderBlob == NULL)  // NB: Pass ID3D10Blob* pErrorBlob to D3DCompile() to get error showing in (const char*)pErrorBlob->GetBufferPointer(). Make sure to Release() the blob!
				return false;
			if (g_pd3dDevice->CreatePixelShader((DWORD*)g_pPixelShaderBlob->GetBufferPointer(), g_pPixelShaderBlob->GetBufferSize(), NULL, &g_pPixelShader) != S_OK)
				return false;
		}

		// Create the blending setup
		{
			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
			g_pd3dDevice->CreateBlendState(&desc, &g_pBlendState);
		}

		// Create the rasterizer state
		{
			D3D11_RASTERIZER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.FillMode = D3D11_FILL_SOLID;
			desc.CullMode = D3D11_CULL_NONE;
			desc.ScissorEnable = true;
			desc.DepthClipEnable = true;
			g_pd3dDevice->CreateRasterizerState(&desc, &g_pRasterizerState);
		}

		// Create depth-stencil State
		{
			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			desc.DepthEnable = false;
			desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
			desc.StencilEnable = false;
			desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.BackFace = desc.FrontFace;
			g_pd3dDevice->CreateDepthStencilState(&desc, &g_pDepthStencilState);
		}

		ImGuiIO& io = ImGui::GetIO();

		//FileHandle font_handle = FileSystem::read_file("Font/DroidSans.ttf");
		//io.Fonts->AddFontFromMemoryTTF(font_handle.buffer, font_handle.size, 16.0f);
		//free(font_handle.buffer);
		io.Fonts->AddFontFromFileTTF("Assets/Font/DroidSans.ttf", 16.0f);

		create_fonts_texture();

		return true;
	}

	void new_frame()
	{
		if (!g_pFontSampler)
			create_device_objects();

		ImGuiIO& io = ImGui::GetIO();

		// Setup display size (every frame to accommodate for window resizing)
		int w, h;
		int display_w, display_h;
		SDL_GetWindowSize(g_Window, &w, &h);
		SDL_GL_GetDrawableSize(g_Window, &display_w, &display_h);
		io.DisplaySize = ImVec2((float)w, (float)h);
		io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

		// Setup time step
		Uint32	time = SDL_GetTicks();
		double current_time = time / 1000.0;
		io.DeltaTime = g_Time > 0.0 ? (float)(current_time - g_Time) : (float)(1.0f / 60.0f);
		g_Time = current_time;

		// Setup inputs
		// (we already got mouse wheel, keyboard keys & characters from SDL_PollEvent())
		int mx, my;
		Uint32 mouseMask = SDL_GetMouseState(&mx, &my);
		if (SDL_GetWindowFlags(g_Window) & SDL_WINDOW_MOUSE_FOCUS)
			io.MousePos = ImVec2((float)mx, (float)my);   // Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
		else
			io.MousePos = ImVec2(-1, -1);

		io.MouseDown[0] = g_MousePressed[0] || (mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;		
		io.MouseDown[1] = g_MousePressed[1] || (mouseMask & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
		io.MouseDown[2] = g_MousePressed[2] || (mouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
		g_MousePressed[0] = g_MousePressed[1] = g_MousePressed[2] = false;

		io.MouseWheel = g_MouseWheel;
		g_MouseWheel = 0.0f;

		// Hide OS mouse cursor if ImGui is drawing it
		SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);

		// Start the frame
		ImGui::NewFrame();
	}

	void initialize()
	{
		RenderDevice& device = context::get_render_device();

		g_Window = context::get_platform().get_window();
		g_pd3dDevice = device.GetD3D11Device();
		g_pd3dDeviceContext = device.GetD3D11DeviceContext();

		ImGuiIO& io = ImGui::GetIO();
		io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
		io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
		io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
		io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
		io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
		io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
		io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
		io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
		io.KeyMap[ImGuiKey_A] = SDLK_a;
		io.KeyMap[ImGuiKey_C] = SDLK_c;
		io.KeyMap[ImGuiKey_V] = SDLK_v;
		io.KeyMap[ImGuiKey_X] = SDLK_x;
		io.KeyMap[ImGuiKey_Y] = SDLK_y;
		io.KeyMap[ImGuiKey_Z] = SDLK_z;

		io.RenderDrawListsFn = render_callback;  
		io.SetClipboardTextFn = set_clipboard_text;
		io.GetClipboardTextFn = get_clipboard_text;
		//io.ClipboardUserData = NULL;
#ifdef _WIN32
		io.ImeWindowHandle = context::get_platform().get_handle_win32();
#endif
		create_device_objects();
	}

	void shutdown()
	{
		InvalidateDeviceObjects();
		ImGui::Shutdown();
	}

	void render()
	{
		ImGui::Render();
	}

} }

#endif
