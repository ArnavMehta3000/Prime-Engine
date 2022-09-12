#include "pch.h"
#include "GraphicsRenderer2D.h"
#include "Prime/Types/VertexBufferTypes.h"
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

		D3D11_INPUT_ELEMENT_DESC inputLayout[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		//D3D11_INPUT_ELEMENT_DESC instanceLayout[] =
		//{
		//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//	//{ "SV_InstanceID", 0, DXGI_FORMAT_R32_UINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 0 },
		//	{ "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_INSTANCE_DATA, 0 },
		//};

		//m_instanceVS.reset(factory->CreateVertexShader(L"Shaders/PrimitiveVertex.cso", instanceLayout, ARRAYSIZE(instanceLayout)));

		VertexShader::VSCompileDesc vsDesc{};
		vsDesc.SourceFile  = L"Shaders/SimpleVertex.hlsl";
		vsDesc.EntryPoint  = "main";
		vsDesc.Profile     = Prime::VertexShader::GetProfile();
		vsDesc.InputDesc   = inputLayout;
		vsDesc.NumElements = ARRAYSIZE(inputLayout);

		PixelShader::PSCompileDesc psDesc{};
		psDesc.SourceFile = L"Shaders/SimplePixel.hlsl";
		psDesc.EntryPoint = "main";
		psDesc.Profile    = Prime::PixelShader::GetProfile();

		m_primitivesVS.reset(factory->CreateVertexShaderFromFile(vsDesc));
		m_primitivesPS.reset(factory->CreatePixelShaderFromFile(psDesc));

		m_primitiveColor = Colors::Magenta;





		/*struct InstanceData
		{
			Vector3 position = Vector3();
		};

		constexpr int rows = 3;
		constexpr int cols = 3;
		std::vector<InstanceData> data(rows * cols);

		int i = 0;
		for (int row = 0; row < rows; row++)
		{
			for (int col = 0; col < cols; col++)
			{
				Vector3 pos((float)row, (float)col, 0.0f);
				data[i].position = pos;
				i++;
			}
		}

		D3D11_BUFFER_DESC instDesc{};
		ZeroMemory(&instDesc, sizeof(D3D11_BUFFER_DESC));
		instDesc.Usage = D3D11_USAGE_DEFAULT;
		instDesc.ByteWidth = sizeof(InstanceData);
		instDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instDesc.CPUAccessFlags = 0;
		instDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA instData{};
		ZeroMemory(&instData, sizeof(D3D11_SUBRESOURCE_DATA));
		instData.pSysMem = &data[0];
		THROW_HR(m_device->CreateBuffer(&instDesc, &instData, m_instanceBuffer.GetAddressOf()), "Failed to create instance buffer");*/


	}


	void GraphicsRenderer2D::DrawQuad()
	{
		Bind(m_quadIB);
		Bind(m_quadVB);
		Bind(m_primitivesVS);
		Bind(m_primitivesPS);
		DrawIndexed(m_quadIB);
	}

	/*void GraphicsRenderer2D::DrawInstancedQuads()
	{
		struct InstanceData
		{
			Vector3 position = Vector3();
		};

		ID3D11Buffer* vertInstbuffer[2] = { m_quadVB->GetCOM().Get(), m_instanceBuffer.Get() };
		UINT strides[2]                 = { sizeof(SimpleVertex), sizeof(InstanceData) };
		UINT offset[2]                  = { 0,0 };

		Bind(m_instanceVS);
		Bind(m_quadIB);
		m_context->IAGetVertexBuffers(0, 2, vertInstbuffer, strides, offset);
		m_context->DrawIndexedInstanced(m_quadIB->GetCount(), 3 * 3, 0, 0, 0);
	}*/
}
