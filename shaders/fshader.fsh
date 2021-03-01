//precision highp float;
uniform highp float u_SourceAlpha;
varying highp vec4 v_AmbientAndDiffuseColor;
varying highp vec4 v_SpecularColor;
varying highp float v_AmbientLightBrightness;
varying highp float v_DiffuseBrightness;
varying highp float v_SpecularBrightness;
varying highp vec2 v_TexCoord;
uniform highp vec2 u_MaskCenter;
uniform highp float u_MaskDiameterSquare;

uniform bool u_TextureEnabled;
uniform bool u_MaskActive;
uniform sampler2D s_Texture;

highp vec2 centerVec;
const highp float c_zero = 0.0;
const highp float c_one = 1.0;

void main(void)
{
    highp float distance;

    if(u_TextureEnabled)
    {
      gl_FragColor = texture2D(s_Texture, v_TexCoord) * (v_AmbientLightBrightness + v_DiffuseBrightness);
      highp vec4 specularColor = v_SpecularBrightness * v_SpecularColor;
      specularColor[3] = gl_FragColor[3]; //take alpha value from texture
      gl_FragColor += specularColor;
      if(gl_FragColor[3] <= 0.01) //Do not process fully transparent pixels
          discard;
      gl_FragColor[3] = u_SourceAlpha; //Do not modulate Alpha! Global SourceAlpha is used for making objects transparent
    }
    else
    {
      gl_FragColor = v_AmbientAndDiffuseColor * (v_AmbientLightBrightness + v_DiffuseBrightness)
                   + v_SpecularBrightness * v_SpecularColor;
      gl_FragColor[3] = u_SourceAlpha;//v_AmbientAndDiffuseColor[3]; //Do not modulate Alpha!
    }

    if(u_MaskActive){ //masking is activated
        centerVec =  gl_FragCoord.xy - u_MaskCenter;
        distance = dot(centerVec, centerVec);
        if(distance > u_MaskDiameterSquare)
            discard;
    }
}
