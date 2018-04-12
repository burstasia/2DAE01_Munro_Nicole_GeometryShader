float4x4 gWorld : WORLD;
float4x4 gWorldViewProj : WORLDVIEWPROJECTION; 
float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);
int gNumSegments;
int gNumCircleSides;
bool gIsLeft;
bool gIsRight;
bool gCrossBeam;
bool gSupportLeft;
bool gSupportRight;
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

    //const static int leftTrackVertsSize = gNumSegments * gNumCircleSides;
    //const static int rightTrackVertsSize = gNumSegments * gNumCircleSides;;
    //const static int crossBeamVertsSize = gNumSegments * 8;

    float3 leftTrackVerts[70];
    float3 rightTrackVerts[70];
    float3 crossBeamVerts[40];
    float3 supportLeftVerts[20];
    float3 supportRightVerts[20];

    float offset = 1.75f;

    //box cross beam vars
    float height = gRadius / 2.0f;
    float width = gRadius * 2.0f;
    float length = offset * 2.0f;
    float boxVerts = 8;

    //plane support
    float widthSupport = gRadius;
    float lengthSupport; // first.y
    //vectors
    float3 first;
    float3 second;
    float3 forwardVec;
    float3 up;
    float3 right;

    for (int i = 0; i <= gNumSegments; i++)
    {
        t = saturate(delta * float(i));
        first = ToBezier(t, p1, p2, p3, p4);
        t += 0.01;
        second = ToBezier(t, p1, p2, p3, p4);

        forwardVec = normalize(second - first);
        up = float3(0, 1, 0);
        right = normalize(cross(up, forwardVec));
        up = normalize(cross(forwardVec, right));

        float currAngle = 0.0f;
        
        //create left track verts
        if (gIsLeft == true)
        {
            for (int l = 0; l <= gNumCircleSides; l++)
            {
                float3 y = gRadius * sin(currAngle) * up;
                float3 x = gRadius * cos(currAngle) * right;

                leftTrackVerts[(gNumCircleSides * i) + l] = y + x + first - (right * 1.75f);

                currAngle += angleIncrement;
            
            }
        }
        //create right track verts
        else if (gIsRight == true)
        {
            for (int t = 0; t <= gNumCircleSides; t++)
            {
                float3 y = gRadius * sin(currAngle) * up;
                float3 x = gRadius * cos(currAngle) * right;

                rightTrackVerts[(gNumCircleSides * i) + t] = y + x + first + (right * 1.75f);

                currAngle += angleIncrement;
            
            }
        }
        // create box verts
        if (gCrossBeam == true)
        {
            
            float3 topRightRight = (first + (forwardVec * width * 0.5f * -1.0f) + (up * height * 0.5f)) + (-1.0f * offset * right);
            float3 topLeftRight = (first + (forwardVec * width * 0.5f) + (up * height * 0.5f)) + (-1.0f * offset * right);
            float3 bottomRightRight = (first + (forwardVec * width * 0.5f * -1.0f) + (up * height * 0.5f * -1.0f)) + (-1.0f * offset * right);
            float3 bottomLeftRight = (first + (forwardVec * width * 0.5f) + (up * height * 0.5f * -1.0f)) + (-1.0f * offset * right);

            float3 topRightLeft = (first + (forwardVec * width * 0.5f * -1.0f) + (up * height * 0.5f)) + (offset * right);
            float3 topLeftLeft = (first + (forwardVec * width * 0.5f) + (up * height * 0.5f)) + (offset * right);
            float3 bottomRightLeft = (first + (forwardVec * width * 0.5f * -1.0f) + (up * height * 0.5f * -1.0f)) + (offset * right);
            float3 bottomLeftLeft = (first + (forwardVec * width * 0.5f) + (up * height * 0.5f * -1.0f)) + (offset * right);

            crossBeamVerts[(i * boxVerts) + 0] = topRightRight;
            crossBeamVerts[(i * boxVerts) + 1] = topLeftRight;
            crossBeamVerts[(i * boxVerts) + 2] = bottomLeftRight;
            crossBeamVerts[(i * boxVerts) + 3] = bottomRightRight;
        
            crossBeamVerts[(i * boxVerts) + 4] = topRightLeft;
            crossBeamVerts[(i * boxVerts) + 5] = topLeftLeft;
            crossBeamVerts[(i * boxVerts) + 6] = bottomLeftLeft;
            crossBeamVerts[(i * boxVerts) + 7] = bottomRightLeft;
            
        }

        if(gSupportLeft)
        {
            float3 rightTop = first + (widthSupport * forwardVec * 0.5f * -1.0f) + (-1.0f * offset * right);
            float3 leftTop = first + (widthSupport * forwardVec * 0.5f) + (-1.0f * offset * right);
            float3 leftBottom = first + (up * (-1.0f * first.y)) + (widthSupport * forwardVec * 0.5f) + (-1.0f * offset * right);
            float3 rightBottom = first + (up * (-1.0f * first.y)) + (widthSupport * forwardVec * 0.5f * -1.0f) + (-1.0f * offset * right);

            supportLeftVerts[(i * 4) + 0] = rightTop;
            supportLeftVerts[(i * 4) + 1] = leftTop;
            supportLeftVerts[(i * 4) + 2] = leftBottom;
            supportLeftVerts[(i * 4) + 3] = rightBottom;

        }
        if (gSupportRight)
        {
            float3 rightTop = first + (widthSupport * forwardVec * 0.5f * -1.0f) + (offset * right);
            float3 leftTop = first + (widthSupport * forwardVec * 0.5f) + (offset * right);
            float3 leftBottom = first + (up * (-1.0f * first.y)) + (widthSupport * forwardVec * 0.5f) + (offset * right);
            float3 rightBottom = first + (up * (-1.0f * first.y)) + (widthSupport * forwardVec * 0.5f * -1.0f) + ( offset * right);

            supportRightVerts[(i * 4) + 0] = rightTop;
            supportRightVerts[(i * 4) + 1] = leftTop;
            supportRightVerts[(i * 4) + 2] = leftBottom;
            supportRightVerts[(i * 4) + 3] = rightBottom;

        }
    }
    // create triangles left Track
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
    
    // create triangles right Track
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

    // create triangles cross beam 
    float3 brown = float3(104.0f / 255.0f, 33.0f / 255.0f, 7.0f / 255.0f);
    if (gCrossBeam == true)
    {
      
        for (int k = 0; k < gNumSegments; k++)
        {
            for (int j = 0; j < (boxVerts / 2); j++)
            {
           
                CreateVertex(triStream, crossBeamVerts[(boxVerts * k) + j], float3(0.65f, 0.65f, 0.65f), brown);
                CreateVertex(triStream, crossBeamVerts[(boxVerts * k) + j + (boxVerts / 2)], float3(0.65f, 0.65f, 0.65f), brown);
                CreateVertex(triStream, crossBeamVerts[(boxVerts * k) + (j + 1) % (boxVerts / 2)], float3(0.65f, 0.65f, 0.65f), brown);
                CreateVertex(triStream, crossBeamVerts[(boxVerts * k) + (j + 1) % (boxVerts / 2) + (boxVerts / 2)], float3(0.65f, 0.65f, 0.65f), brown);

            }
            
            triStream.RestartStrip();
        }

    }

    // create triangles support
    if(gSupportLeft == true)
    {
        for (int k = 0; k < gNumSegments; k++)
        {
            CreateVertex(triStream, supportLeftVerts[k * 4], float3(0.65f, 0.65f, 0.65f), brown);
            CreateVertex(triStream, supportLeftVerts[(k * 4) + 3], float3(0.65f, 0.65f, 0.65f), brown);
            CreateVertex(triStream, supportLeftVerts[(k * 4) + 1], float3(0.65f, 0.65f, 0.65f), brown);
            CreateVertex(triStream, supportLeftVerts[(k * 4) + 2], float3(0.65f, 0.65f, 0.65f), brown);

            triStream.RestartStrip();
        }
    }

    if (gSupportRight == true)
    {
        for (int k = 0; k < gNumSegments; k++)
        {
            CreateVertex(triStream, supportRightVerts[k * 4], float3(0.65f, 0.65f, 0.65f), brown);
            CreateVertex(triStream, supportRightVerts[(k * 4) + 3], float3(0.65f, 0.65f, 0.65f), brown);
            CreateVertex(triStream, supportRightVerts[(k * 4) + 1], float3(0.65f, 0.65f, 0.65f), brown);
            CreateVertex(triStream, supportRightVerts[(k * 4) + 2], float3(0.65f, 0.65f, 0.65f), brown);

            triStream.RestartStrip();
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



