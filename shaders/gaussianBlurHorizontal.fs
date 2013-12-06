// Fake Gaussian Blur
// To use for a glow:
// 1. Render glow sources
// 2. Blur filter the resulting image
// 2.5 Can separate horizontal and vertical blur filter into separate passes; just set blend function to GL_ONE, GL_ONE
// 3. Save result to a temporary texture object
// 4. Blend result of glow with non-glow image for final result

uniform sampler2D tex;
uniform float blurSize;
varying vec2 f_texcoord;

// TODO allow setting size of Gaussian blur pixel neighborhood
// TODO allow setting of amplitude and width of Gaussian function
// TODO add attenuation factor for color
// TODO separate into separate vertical/horizontal passes

void main()
{		
	vec4 sum = vec4(0.0);

	// Vertical blur
	sum += texture2D(tex, vec2(f_texcoord.x - 4.0*blurSize, f_texcoord.y))*0.05;
	sum += texture2D(tex, vec2(f_texcoord.x - 3.0*blurSize, f_texcoord.y))*0.09;
	sum += texture2D(tex, vec2(f_texcoord.x - 2.0*blurSize, f_texcoord.y))*0.12;
	sum += texture2D(tex, vec2(f_texcoord.x - blurSize, f_texcoord.y))*0.15;
	sum += texture2D(tex, vec2(f_texcoord.x, f_texcoord.y))*0.16;
	sum += texture2D(tex, vec2(f_texcoord.x + blurSize, f_texcoord.y))*0.15;
	sum += texture2D(tex, vec2(f_texcoord.x + 2.0*blurSize, f_texcoord.y))*0.12;
	sum += texture2D(tex, vec2(f_texcoord.x + 3.0*blurSize, f_texcoord.y))*0.09;
	sum += texture2D(tex, vec2(f_texcoord.x + 4.0*blurSize, f_texcoord.y))*0.05;

	gl_FragColor = sum;


}