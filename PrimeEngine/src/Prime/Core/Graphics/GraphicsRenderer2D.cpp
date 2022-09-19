#include "pch.h"
#include "GraphicsRenderer2D.h"
#include "Prime/Types/VertexBufferTypes.h"
#include "Prime/Types/ConstantbufferTypes.h"
#include "Prime/Core/Graphics/GraphicsFactory.h"
#include <SpriteBatch.h>


namespace Prime
{
	Prime::GraphicsRenderer2D::GraphicsRenderer2D()
	{
		m_device = nullptr;
		m_context = nullptr;
	}

	Prime::GraphicsRenderer2D::~GraphicsRenderer2D()
	{
	}

	void Prime::GraphicsRenderer2D::Init2D(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		m_device = device;
		m_context = context;

		this->Init(device, context);
		InitQuad();

		BindDefaults();
	}

	void GraphicsRenderer2D::InitQuad()
	{
		auto factory = Locator::ResolveService<GraphicsFactory>();

		const TexturedVertex quadVerts[] =
		{
			{ -1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
			{  1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
			{ -1.0f,-1.0f, 0.0f, 0.0f, 0.0f },
			{  1.0f,-1.0f, 0.0f, 1.0f, 0.0f },
		};
		m_quadVB.reset(factory->CreateVertexBuffer(quadVerts, UINT(sizeof(TexturedVertex)), ARRAYSIZE(quadVerts)));

		DWORD quadIndices[] = { 0, 1, 2, 2, 1, 3 };
		m_quadIB.reset(factory->CreateIndexBuffer(quadIndices, ARRAYSIZE(quadIndices)));
		D3D11_INPUT_ELEMENT_DESC primitiveInputLayout[] =
		{
			{ "POSITION",      0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",      0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			//{ "SV_InstanceID", 0, DXGI_FORMAT_R32_FLOAT,       0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		VertexShader::VSCompileDesc vd{}; 
		vd.SourceFile  = L"Shaders/PrimitiveVS.hlsl";
		vd.EntryPoint  = Prime::VertexShader::GetDeafultEntryPoint();
		vd.Profile     = Prime::VertexShader::GetProfile();
		vd.InputDesc   = primitiveInputLayout;
		vd.NumElements = ARRAYSIZE(primitiveInputLayout);
		m_primitiveVS.reset(factory->CreateVertexShaderFromFile(vd));

		PixelShader::PSCompileDesc pd{};
		pd.SourceFile = L"Shaders/PrimitivePS.hlsl";
		pd.EntryPoint = Prime::PixelShader::GetDeafultEntryPoint();
		pd.Profile    = Prime::PixelShader::GetProfile();
		m_primitivePS.reset(factory->CreatePixelShaderFromFile(pd));


		cbFrame.reset(factory->CreateConstantBuffer<PrimitivesFrameBufferVS>());
		cbObject.reset(factory->CreateConstantBuffer<PrimitivesObjectBufferVS>());
	
		
		// Create default 1x1 white texture
		const uint32_t pixel = 0xffffffff;
		
		D3D11_SUBRESOURCE_DATA initData = { &pixel, sizeof(uint32_t), 0 };
		
		D3D11_TEXTURE2D_DESC desc{};
		ZeroMemory(&desc, sizeof(desc));
		desc.Width            = desc.Height = desc.MipLevels = desc.ArraySize = 1;
		desc.Format           = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.Usage            = D3D11_USAGE_IMMUTABLE;
		desc.BindFlags        = D3D11_BIND_SHADER_RESOURCE;

		ComPtr<ID3D11Texture2D> tex;
		THROW_HR(m_device->CreateTexture2D(&desc, &initData, tex.GetAddressOf()), "Failed to create 1x1 white pixel texture");

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		THROW_HR(m_device->CreateShaderResourceView(tex.Get(), &srvDesc, m_whiteTexture.GetAddressOf()), "Failed to create 1x1 white pixel shader resource view");
	}

	void GraphicsRenderer2D::BeginQuadBatch(const Matrix& view, const Matrix& proj)
	{
		cbFrame->Data.ViewMatrix       = view.Transpose();
		cbFrame->Data.ProjectionMatrix = proj.Transpose();

		UpdateConstantBuffer(cbFrame);
		
		Bind(ShaderType::PixelShader, s_samplerLinearWrap);
		m_context->PSSetShaderResources(0, 1, m_whiteTexture.GetAddressOf());
		Bind(s_blendStateAlphaEnabled);
		Bind(ShaderType::VertexShader, cbFrame, 0);
		Bind(ShaderType::VertexShader, cbObject, 1);
		Bind(m_primitiveVS);
		Bind(m_primitivePS);
		Bind(m_quadIB);
		Bind(m_quadVB);
	}
	void GraphicsRenderer2D::DrawQuadBatch(const Matrix& world, const Color& col)
	{
		cbObject->Data.WorldMatrix = world.Transpose();
		cbObject->Data.Color       = col;

		UpdateConstantBuffer(cbObject);
		
		DrawIndexed(m_quadIB);
	}
}
