float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
int gNumSegments;
int gNumCircleSides;
bool gIsLeft;
bool gIsRight;
float gRadius;
static const float TWO_PI = 6.28318530718f;

struct VS_INPUT{
	float3 pos[4] : BEZIER;
};

struct GS_OUTPUT{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
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

void CreateVertex(inout TriangleStream<GS_OUTPUT> triStream, float3 pos, float3 normal, float3 color)
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
[maxvertexcount(93)]
void GS(point VS_INPUT input[1], inout TriangleStream<GS_OUTPUT> triStream)
{
    float3 p1 = input[0].pos[0];
    float3 p2 = input[0].pos[1];
    float3 p3 = input[0].pos[2];
    float3 p4 = input[0].pos[3];

    float delta = 1.0f / gNumSegments;
    float t;

    float angleIncrement = TWO_PI / (float) gNumCircleSides;

    float3 leftTrackVerts[60];
    float3 rightTrackVerts[70];

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

        float radius = 0.5f;

        //create circle
        float currAngle = 0.0f;
        
        if(gIsLeft == true)
         {
            for (int l = 0; l <= gNumCircleSides; l++)
            {
                float3 y = radius * sin(currAngle) * up;
                float3 x = radius * cos(currAngle) * right;

                leftTrackVerts[(gNumCircleSides * i) + l] = y + x + first + (right * 1.75f);

                currAngle += angleIncrement;
            
            }
        }
        else if (gIsRight == true)
        {
            for (int t = 0; t <= gNumCircleSides; t++)
            {
                float3 y = radius * sin(currAngle) * up;
                float3 x = radius * cos(currAngle) * right;

                rightTrackVerts[(gNumCircleSides * i) + t] = y + x + first - (right * 1.75f);

                currAngle += angleIncrement;
            
            }
        }
        
    
        //TODO: create box
    }

    if (gIsLeft == true)
    {

        for (int k = 0; k < gNumSegments; k++)
        {
       
            for (int j = 0; j < gNumCircleSides; j++)
            {
           
                CreateVertex(triStream, leftTrackVerts[(gNumCircleSides * k) + j], float3(0.65f, 0.65f, 0.65f), float3(0.65f, 0.65f, 0.65f));
                CreateVertex(triStream, leftTrackVerts[(gNumCircleSides * k) + j + gNumCircleSides], float3(0.65f, 0.65f, 0.65f), float3(0.65f, 0.65f, 0.65f));
                CreateVertex(triStream, leftTrackVerts[(gNumCircleSides * k) + (j + 1) % gNumCircleSides], float3(0.65f, 0.65f, 0.65f), float3(0.65f, 0.65f, 0.65f));
                CreateVertex(triStream, leftTrackVerts[(gNumCircleSides * k) + (j + 1) % gNumCircleSides + gNumCircleSides], float3(0.65f, 0.65f, 0.65f), float3(0.65f, 0.65f, 0.65f));

                triStream.RestartStrip();
            }
      
        }
        }
     //TODO: create triangles left Track
    else if (gIsRight == true)
    {
        for (int k = 0; k < gNumSegments; k++)
        {
       
            for (int j = 0; j < gNumCircleSides; j++)
            {
           
                CreateVertex(triStream, rightTrackVerts[(gNumCircleSides * k) + j], float3(0.65f, 0.65f, 0.65f), float3(0.65f, 0.65f, 0.65f));
                CreateVertex(triStream, rightTrackVerts[(gNumCircleSides * k) + j + gNumCircleSides], float3(0.65f, 0.65f, 0.65f), float3(0.65f, 0.65f, 0.65f));
                CreateVertex(triStream, rightTrackVerts[(gNumCircleSides * k) + (j + 1) % gNumCircleSides], float3(0.65f, 0.65f, 0.65f), float3(0.65f, 0.65f, 0.65f));
                CreateVertex(triStream, rightTrackVerts[(gNumCircleSides * k) + (j + 1) % gNumCircleSides + gNumCircleSides], float3(0.65f, 0.65f, 0.65f), float3(0.65f, 0.65f, 0.65f));

                triStream.RestartStrip();
            }
      
        }
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
	
	//HalfLambert Diffuse :)
	float diffuseStrength = dot(input.normal, -gLightDirection);
	diffuseStrength = diffuseStrength * 0.5 + 0.5;
	diffuseStrength = saturate(diffuseStrength);
	color_rgb = color_rgb * diffuseStrength;
	
	return float4( color_rgb , 1 );
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



