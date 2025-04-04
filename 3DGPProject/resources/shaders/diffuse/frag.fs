varying vec2 v_TexCoord;
varying vec3 v_Normal;
varying vec3 v_FragPos;
uniform sampler2D u_Texture;

void main()
{
    vec3 lightPos = vec3(10, 10, 10);
    vec3 diffuseColor = vec3(1, 1, 1);

    vec3 N = normalize(v_Normal);
    vec3 lightDir = normalize(lightPos - v_FragPos);
    float diff = max(dot(N, lightDir), 0.0);
    vec3 diffuse = diffuseColor * diff;

    vec3 lighting = diffuse;

    vec4 tex = texture2D(u_Texture, v_TexCoord);
    gl_FragColor = vec4(lighting, 1) * tex;
}