//───────────────────────────────────────
// テクスチャ＆サンプラーデータのグローバル変数定義
//───────────────────────────────────────
Texture2D g_texture : register(t0); //テクスチャー
SamplerState g_sampler : register(s0); //サンプラー

//───────────────────────────────────────
 // コンスタントバッファ
// DirectX 側から送信されてくる、ポリゴン頂点以外の諸情報の定義
//───────────────────────────────────────
cbuffer global : register(b0)
{
    //変換行列、視点、光源
    float4x4 matWVP; // ワールド・ビュー・プロジェクションの合成行列
    float4x4 matW; //法線をワールド座標に対応させる行列＝回転＊
    float4x4 matNormal;
    float4 diffuseColor; //拡散反射係数
    float4 factor;
    float4 ambientColor;
    float4 specularColor;
    float4 lightVec;
    float4 eyePosition;
    float shininess;
    bool isTextured; //texが貼られているか
};

cbuffer stage : register(b1)
{
    float4 casterPos; // プレイヤーのXZ座標
    float4 shadowParams; // (softness, alphaScale, unused, playerHeightY)
}

//───────────────────────────────────────
// 頂点シェーダー出力＆ピクセルシェーダー入力データ構造体
//───────────────────────────────────────
struct VS_OUT
{
    float4 wpos : POSITION0;
    float4 pos : SV_POSITION; //位置
    float2 uv : TEXCOORD; //UV座標
    float4 cos_alpha : COLOR; //色（明るさ）
    float4 normal : NORMAL;
    float4 eyev : POSITION1;
};

//───────────────────────────────────────
// 頂点シェーダ
//───────────────────────────────────────
VS_OUT VS(float4 pos : POSITION, float4 uv : TEXCOORD, float4 normal : NORMAL)
{
	//ピクセルシェーダーへ渡す情報
    VS_OUT outData;

	//ローカル座標に、ワールド・ビュー・プロジェクション行列をかけて
	//スクリーン座標に変換し、ピクセルシェーダーへ
    outData.pos = mul(pos, matWVP);
    outData.wpos = mul(pos, matW);
    outData.uv = uv;
    
    //float4 light = float4(1, 1, -1, 0); //光源ベクトルの逆ベクトル
    float4 light = lightVec;
    light = normalize(light); //単位ベクトル化

    normal = mul(normal, matNormal);
    outData.normal = normal;
    outData.cos_alpha = clamp(dot(normal, light), 0, 1);
    outData.eyev = eyePosition - mul(pos, matW);
    
	//まとめて出力
    return outData;
}

//───────────────────────────────────────
// ピクセルシェーダ
//───────────────────────────────────────
float4 PS(VS_OUT inData) : SV_Target
{
    float4 ambentSource = { 0.5, 0.5, 0.5, 1.0 }; //環境光の強さ
    float4 diffuse;
    float4 ambient;
    
    float3 dir = normalize(lightVec.xyz - inData.wpos.xyz);
    float4 r = reflect(normalize(inData.normal), normalize(float4(-dir, 1)));
    float4 specular = pow(saturate(dot(r, normalize(inData.eyev))), shininess) * specularColor;
    
    if (isTextured == false)
    {
        diffuse = diffuseColor * inData.cos_alpha * factor.x;
        ambient = diffuseColor * ambentSource * factor.x;
    }
    else
    {
        diffuse = g_texture.Sample(g_sampler, inData.uv) * inData.cos_alpha * factor.x;
        ambient = g_texture.Sample(g_sampler, inData.uv) * ambentSource * factor.x;;
    }
    
    float2 casterXZ = casterPos.xz;
    float2 pixelXZ = inData.wpos.xz;

    float2 diff = pixelXZ - casterXZ;
    float distSq = dot(diff, diff);

    float softness = shadowParams.x;
    float alphaScale = shadowParams.y;
    float heightY = shadowParams.w;

    float heightRatio = saturate(heightY / 2.0f); // 最大ジャンプ2.0f想定
    float radius = lerp(0.4f, 0.8f, heightRatio);
    float alpha = lerp(0.6f, 0.1f, heightRatio);

    float shadowAlpha = saturate((radius * radius - distSq) * softness) * alpha;

    // 丸影を黒で合成（必要に応じて色も乗せられる）
    float4 shadowColor = float4(0, 0, 0, shadowAlpha);
    return lerp(diffuse + ambient + specular, shadowColor, shadowAlpha);

    //return diffuse + ambient + specular;
}