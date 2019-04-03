#include <iostream>

#include "Ridge.h"

#include "Ridge/Debug/ImGuiLayer.h"

#include "Ridge/Graphics/VertexArray.h"
#include "Ridge/Graphics/VertexBuffer.h"
#include "Ridge/Graphics/IndexBuffer.h"
#include "Ridge/Graphics/Shader.h"

#include "Ridge/Math/Vector.h"

using namespace std;
using namespace Ridge;
using namespace Ridge::Graphics;

class TestLayer : public Layer
{

public:
	virtual void OnAttach( ) override
	{
		Vector3f vertices[ ] = {
			{-0.5f, -0.5f, 0.0f}, {1, 0, 0},
			{ 0.5f, -0.5f, 0.0f}, {0, 1, 0},
			{ 0.0f,  0.5f, 0.0f}, {0, 0, 1}
		};

		uint16 indices[ ] = {
			0, 1, 2
		};

		BufferLayout layout;
		layout.Push<Vector3f>( "position" );
		layout.Push<Vector3f>( "color" );

		VertexBuffer * vbo = VertexBuffer::Create( BufferUsage::Static );
		vbo->SetData( 6 * sizeof( Vector3f ), &vertices[ 0 ] );

		m_vao = std::unique_ptr<VertexArray>( VertexArray::Create( ) );
		m_vao->PushBuffer( vbo, layout );

		m_ibo = std::unique_ptr<IndexBuffer>( IndexBuffer::Create( indices, 3 ) );

		m_minimalShader = std::unique_ptr<ShaderProgram>( Context::CreateShaderProgram( ) );
		m_minimalShader->LoadFromFile( "./res/Shaders/OpenGL/minimal.glsl" );
	}


	virtual void OnDetach( ) override
	{

	}


	virtual void OnUpdate( double deltaTime ) override
	{
	}


	virtual void OnRender( double deltaTime ) override
	{
		Window & window = Application::Get( ).GetWindow( );
		//float aspectRatio = static_cast<float>( window.GetWidth( ) ) / window.GetHeight( );
		float width = static_cast<float>( window.GetWidth( ) );
		float height = static_cast<float>( window.GetHeight( ) );

		Matrix4f projection;
		if( width >= height )
		{
			float aspect = width / height;
			projection = Matrix4f::Orthographic( -aspect, aspect, -1.0f, 1.0f, 0.1f, 100.0f );
		}
		else
		{
			float aspect = height / width;
			projection = Matrix4f::Orthographic( -1.0f, 1.0f, -aspect, aspect, 0.1f, 100.0f );
		}
		Matrix4f model = Matrix4f::Translation( { 0.0f, 0.0f, 1.0f } );
		Matrix4f mvp = projection * model;

		m_minimalShader->Bind( );
		m_minimalShader->SetUniform( "u_MVP", mvp );
		m_vao->Bind( );
		m_ibo->Bind( );
		m_vao->Draw( 3 );
	}


	virtual void OnEvent( Event & event ) override
	{
	}

private:
	std::unique_ptr<VertexArray> m_vao;
	std::unique_ptr<IndexBuffer> m_ibo;
	std::unique_ptr<ShaderProgram> m_minimalShader;

};

class Sandbox : public Application
{
public:
	Sandbox( )
	{
	}
	~Sandbox( )
	{
	}
	bool OnInit( ) override
	{
		auto result = Application::OnInit( );
		RIDGE_INFO( "Welcome to the Sandbox!" );
		PushLayer( new TestLayer( ) );
		PushOverlay( new ImGuiLayer( ) );

		return result;
	}
private:
};

Application * CreateApplication( )
{
	return new Sandbox( );
}