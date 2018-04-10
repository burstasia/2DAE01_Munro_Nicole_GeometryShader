float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
int gNumSegments = int(10);
int gNumCircleSides = int(8);

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

float3 ToBezier(float t, float3 P0, float3 P1, float3 P2, float3 P3)
{
   
    float t2 = t * t;

    float t3 = t * t * t;

    return (pow((1.0f - t), 3) * P0) +
            (pow((1 - t), 2) * 3 * t * P1) +
            ((1 - t) * 3 * t2 * P2) +
            (t3 * P3);

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
[maxvertexcount(90)]
void GS(point VS_INPUT input[1], inout LineStream<GS_OUTPUT> triStream)
{
    float3 p1 = input[0].pos[0];
    float3 p2 = input[0].pos[1];
    float3 p3 = input[0].pos[2];
    float3 p4 = input[0].pos[3];

    float delta = 1.0f / gNumSegments;
    float t;

    //float angleIncrement = 
    for (int i = 0; i <= gNumSegments; i++)
    {
        t = saturate(delta * float(i));
        float3 first = ToBezier(t, p1, p2, p3, p4);
        t += 0.01;
        float3 second = ToBezier(t, p1, p2, p3, p4);

        float3 forwardVec = normalize(second - first);
        float3 up = float3(0, 1, 0);
        float3 right = normalize(cross(up, forwardVec));
        up = normalize(cross(forwardVec, right));

        float radius = 1.0f;

        //create circle
        float currAngle = 0.0f;
        
        for (int i = 0; i < 9; i++)
        {
            float3 y = radius * sin(currAngle) * up;
            float3 x = radius * cos(currAngle) * right;

            CreateVertex(triStream, y + x + first, float3(0, 0, 0), float4(1, 0, 0, 1));

            currAngle += 0.78f;
            
        }
        triStream.RestartStrip();
    }

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



