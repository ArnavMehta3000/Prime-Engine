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

		const SimpleVertex quadVerts[] =
		{
			{ -1.0f, 1.0f, 0.0f },
			{  1.0f, 1.0f, 0.0f },
			{ -1.0f,-1.0f, 0.0f },
			{  1.0f,-1.0f, 0.0f },
		};
		m_quadVB.reset(factory->CreateVertexBuffer(quadVerts, UINT(sizeof(SimpleVertex)), ARRAYSIZE(quadVerts)));

		DWORD quadIndices[] = { 0, 1, 2, 2, 1, 3 };
		m_quadIB.reset(factory->CreateIndexBuffer(quadIndices, ARRAYSIZE(quadIndices)));
		D3D11_INPUT_ELEMENT_DESC primitiveInputLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D10_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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
	}

	void GraphicsRenderer2D::BeginQuadBatch(const Matrix& view, const Matrix& proj)
	{
		cbFrame->Data.ViewMatrix       = view.Transpose();
		cbFrame->Data.ProjectionMatrix = proj.Transpose();

		UpdateConstantBuffer(cbFrame);

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
