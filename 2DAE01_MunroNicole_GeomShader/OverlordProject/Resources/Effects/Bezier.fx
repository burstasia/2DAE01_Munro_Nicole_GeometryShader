float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
int gNumSegments = int(20);

struct VS_INPUT{
	float3 pos[4] : BEZIER;
};

struct GS_OUTPUT{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float3 normal : NORMAL;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

RasterizerState NoCulling
{
	CullMode = NONE;
};

BlendState EnableBlending
{
	BlendEnable[0] = TRUE;
	SrcBlend = SRC_ALPHA;
	DestBlend = INV_SRC_ALPHA;
};


float3 ToBezier(float delta, int i,  float3 P0, float3 P1, float3 P2, float3 P3)
{
    float t = saturate(delta * float(i));

    float t2 = t * t;

    float one_minus_t = 1.0 - t;
    float one_minus_t2 = 1.0 - t2;

    

    return (P0 * one_minus_t2 * one_minus_t 
            + P1 * 3.0 * t * one_minus_t2 
            + P2 * 3.0 * t2 * one_minus_t 
            + P3 * t2 * t);
}

void CreateVertex(inout LineStream<GS_OUTPUT> triStream, float3 pos, float3 normal, float4 color)
{
    GS_OUTPUT temp = (GS_OUTPUT) 0;

    temp.pos = mul(float4(pos, 1), gWorldViewProj);

    temp.normal = mul(normal, (float3x3) gWorld);

    temp.color = color;

    triStream.Append(temp);

}

//--------------------------------------------------------------------------------------
// Geometry Shader
//--------------------------------------------------------------------------------------
[maxvertexcount(45)]
void GS(point VS_INPUT input[1], inout LineStream<GS_OUTPUT> triStream)
{
    //CreateVertex(triStream, input[0].pos[0], float3(0, 0, 0), float4(1, 1, 1, 1));
    //CreateVertex(triStream, input[0].pos[1], float3(0, 0, 0), float4(1, 1, 1, 1));
    //CreateVertex(triStream, input[0].pos[2], float3(0, 0, 0), float4(1, 1, 1, 1));
    //CreateVertex(triStream, input[0].pos[3], float3(0, 0, 0), float4(1, 1, 1, 1));

    
    
    for (int i = 0; i <= gNumSegments; i++)
    {
        
        CreateVertex(triStream, ToBezier(1.0f / gNumSegments, i, input[0].pos[0], input[0].pos[1], input[0].pos[2], input[0].pos[3]), float3(0, 0, 0), float4(1, 0, 1, 1));

    }
   // triStream.RestartStrip();

}

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_INPUT VS(VS_INPUT input)
{
  
	return input;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(GS_OUTPUT input) : SV_TARGET
{

	float3 color_rgb= input.color.rgb;
	float color_a = input.color.a;
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;
	
	return float4( color_rgb , color_a );
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
    pass P0
    {
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);

        SetVertexShader( CompileShader( vs_4_0, VS() ) );
        SetGeometryShader(CompileShader(gs_4_0, GS()));
        SetPixelShader( CompileShader( ps_4_0, PS() ) );
    }
}



