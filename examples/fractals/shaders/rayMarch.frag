#version 450
precision highp float;

layout(binding = 0) uniform UBO {
  // Ray marching
  float minDist;

  // Camera
  mat4 view;
  vec4 cameraOrigin;
  float fov;
  float zoom;
  float aspectRatio;
  float viewportWidth;
  float viewportHeight;

  // Shading
  float dirLightShading;
  vec3 dirLightColor;
  float dirIntensity;
  vec3 specularColor;
  float specularIntensity;

  vec3 backgroundColor;
  float shadowSharpness;

  // Fractals
  int fractalType;
  int iterations;
  float fractalPower;

  // Effects
  vec3 glowColor;
  float glowIntensity;
  vec3 AOColor;
  float AOIntensity;
  float fogCurve;
  float fogStartDist;
} settings;

layout (location = 0) in vec2 inUV;
layout (location = 0) out vec4 outFragColor;

//------------------------------------------
// Math
//------------------------------------------
#define FLT_MAX 3.402823466e+38
const float PI = 3.14159265359;
const float DEG_TO_RAD = PI / 180.0;

float rand(vec2 co)
{
  return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float modul1D(float p, float modulo) {
  return mod(p, modulo) - modulo/2.0;
}

vec3 modul3D(vec3 p, float modulo) {
    return mod(p, modulo) - modulo/2.0;
}

vec3 RotateZ(vec3 p, float a)
{
   float c,s;
   vec3 q=p;
   c = cos(a);
   s = sin(a);
   p.x = c * q.x - s * q.y;
   p.y = s * q.x + c * q.y;
   return p;
}


//------------------------------------------
// Ray marching settings
//------------------------------------------
const highp float ANTIALISING_SAMPLES = 1.0;
const highp float MAX_STEPS = 256.0;
highp float MIN_DIST = settings.minDist;
const highp float MAX_DIST = 100.0;
// const float EPSILON = 0.0001;
highp float EPSILON = MIN_DIST;
const highp float IMAGE_PLANE = 1.0;

vec3 dirLight1 = vec3(1.0, -1.0, -1.0);
vec3 dirLight2 = vec3(-1.0, -1.0, -1.0);
vec3 dirLight3 = vec3(1.0, 1.0, -1.0);
vec3 dirLight4 = vec3(-1.0, 1.0, -1.0);

//------------------------------------------
// Folding operators
//------------------------------------------
// https://github.com/HackerPoet/MarbleMarcher/blob/master/assets/frag.glsl
vec3 boxFold(vec3 z, vec3 r) {
	return clamp(z.xyz, -r, r) * 2.0 - z.xyz;
}

// http://www.fractalforums.com/fragmentarium/fragmentarium-an-ide-for-exploring-3d-fractals-and-other-systems-on-the-gpu/15/
void sphereFold(inout vec3 z, inout float dz) {

    float fixedRadius2 = .6 + 4.* cos(20./8.) + 4.;
    float minRadius2 = 0.3;
	float r2 = dot(z,z);
	if (r2< minRadius2) {
		float temp = (fixedRadius2/minRadius2);
		z*= temp;
		dz*=temp;
	}
    else if (r2<fixedRadius2) {
		float temp =(fixedRadius2/r2);
		z*=temp;
		dz*=temp;
	}
}

// https://github.com/HackerPoet/MarbleMarcher/blob/master/assets/frag.glsl
vec3 mengerFold(vec3 z) {
	float a = min(z.x - z.y, 0.0);
	z.x -= a;
	z.y += a;
	a = min(z.x - z.z, 0.0);
	z.x -= a;
	z.z += a;
	a = min(z.y - z.z, 0.0);
	z.y -= a;
	z.z += a;
    return z;
}

//------------------------------------------
// Distance estimators
//------------------------------------------
float deSphere(vec3 p, vec3 c, float r){
  return length(p - c) - r;
}

float deBox(vec3 p, vec3 b)
{
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

float deTorus(vec3 p, vec2 t)
{
  vec2 q = vec2(length(p.xz)-t.x,p.y);
  return length(q)-t.y;
}

// http://blog.hvidtfeldts.net/index.php/2011/08/distance-estimated-3d-fractals-iii-folding-space/
float deSerpinski(vec3 p)
{
  // const int Iterations = 14;
  const float Scale = 1.85;
  const float Offset = 2.0;

  vec3 a1 = vec3(1,1,1);
	vec3 a2 = vec3(-1,-1,1);
	vec3 a3 = vec3(1,-1,-1);
	vec3 a4 = vec3(-1,1,-1);
	vec3 c;
	float dist, d;
	for (int n = 0; n < settings.iterations; n++)
	{
      if(p.x+p.y<0.) p.xy = -p.yx; // fold 1
      if(p.x+p.z<0.) p.xz = -p.zx; // fold 2
      if(p.y+p.z<0.) p.zy = -p.yz; // fold 3
      p = p*Scale - Offset*(Scale-1.0);
	}
	return length(p) * pow(Scale, -float(settings.iterations));
}

// https://github.com/Shroomisnottaken/3dRaymarcherFramework/blob/master/RaymarchingGLSL/RaymarchingGLSL/GLSLfragmentShader.txt
float deMenegerBox(vec3 p)
{
   float d = deBox(p,vec3(1.0));

   float s = 2.67;
   for( int m=0; m<settings.iterations; m++ )
   {
      vec3 a = mod( p*s, 2.0 )-1.0;
      s *= 3.0;
      vec3 r = abs(1.0 - 3.0*abs(a));

      float da = max(r.x,r.y);
      float db = max(r.y,r.z);
      float dc = max(r.z,r.x);
      float c = (min(da,min(db,dc))-1.0)/s;

      d = max(d,c);
   }

   return d;
}

float deMandelBox(vec3 z)
{
  // float Iterations = 10.0;
  float Scale = 4.0;
	vec3 offset = z;
	float dr = 1.0;
	for (float n = 0.; n < settings.iterations; n++) {
    z = mengerFold(z);
    z = boxFold(z, vec3(2.));       // Reflect
    sphereFold(z, dr);    // Sphere Inversion
    z.xz = -z.zx;
		z = boxFold(z, vec3(1.));       // Reflect

		sphereFold(z, dr);    // Sphere Inversion
    z=Scale*z + offset;  // Scale & Translate
    dr = dr*abs(Scale)+1.0;
	}

	float r = length(z);
	return r/abs(dr);
}

// http://blog.hvidtfeldts.net/index.php/2011/09/distance-estimated-3d-fractals-v-the-mandelbulb-different-de-approximations/
float deMandleBulb(vec3 pos)
{
  //  const int Iterations = 12;
   const float Bailout = 8.0;
  //  float Power = 8.0;

	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < settings.iterations; i++)
	{
		r = length(z);
		if (r > Bailout) break;   // TODO: test if better to continue loop and if() rather than break?

		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = atan(z.y,z.x);
		dr = pow(r, settings.fractalPower-1.0)*settings.fractalPower*dr + 1.0;

		// scale and rotate the point
		float zr = pow(r,settings.fractalPower);
		theta = theta*settings.fractalPower;
		phi = phi*settings.fractalPower;

		// convert back to cartesian coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z += pos;
	}
	return 0.5*log(r)*r/dr;
}

// http://www.iquilezles.org/www/articles/menger/menger.htm
float deMenegerSponge(vec3 p)
{
   float d = deBox(p, vec3(1.0));
  //  const int MENGER_ITERATIONS = 4;

   float s = 1.0;
   for (int m=0; m<settings.iterations; m++)
   {
      p += 0.003;   // translate out slightly - makes more interesting....
      p = RotateZ(p, PI/4.0);

      vec3 a = mod(p*s, 2.0) - 1.0;
      s *= float(settings.iterations);
      vec3 r = abs(1.0 - 3.0*abs(a));

      float da = max(r.x,r.y);
      float db = max(r.y,r.z);
      float dc = max(r.z,r.x);
      float c = (min(da,min(db,dc)) - 1.0)/s;

      d = max(d,c);
   }

   return d;
}

float sceneDE(vec3 p){
  float fractal;

  switch(settings.fractalType) {
    case 0:
      fractal = deMenegerBox(p);
      break;
    case 1:
      fractal = deMandelBox(p);
      break;
    case 2:
      fractal = deMandleBulb(p);
      break;
    case 3:
      fractal = deMenegerSponge(p);
      break;
    default:
      fractal = deSerpinski(p);
  }

  return fractal;
}

//------------------------------------------
// Ray marching code
//------------------------------------------
struct Ray {
  vec3 origin;
  vec3 direction;
};

Ray generateViewRay(vec3 lookFrom, vec2 uv) {
  // Camera transform
  vec3 lookAt = vec3(0.0);
  lookFrom = vec3(settings.view * vec4(lookFrom, 0.0));

  // Camera coordinate system
  vec3 forward = normalize(lookAt - lookFrom);
  vec3 right = normalize(cross(vec3(0.0, 1.0, 0.0), forward));
  vec3 up = cross(forward, right);

  // Zoom
  lookFrom += -forward * settings.zoom;

  // Fov
  float radFov = settings.fov * DEG_TO_RAD;
  float viewportWidth = 2.0 * tan(radFov/2.0);
  // float viewportHeight = viewportWidth * settings.aspect_ratio;
  float viewportHeight = viewportWidth * settings.aspectRatio;

  // Generate ray
  vec3 screenCenter = lookFrom + IMAGE_PLANE * forward;
  vec3 pixelPos = screenCenter + uv.x * right * viewportWidth + uv.y * up * viewportHeight;

  return Ray(lookFrom, normalize(pixelPos - lookFrom));
}

// Other simpler techniques:
// col *= (1.0 - (1.0 / (1.0 + rayMarchSteps)));
// col *= (1.0 - rayMarchSteps/MAX_STEPS);
float CalcAO(vec3 p, vec3 n)
{
   int AO_SAMPLES = 5;
   float r = 0.0;
   float w = 1.0;
   for (int i=1; i<=AO_SAMPLES; i++)
   {
      float d0 = float(i) * 0.3;
      r += w * (d0 - sceneDE(p + n * d0));
      w *= 0.5;
   }
   return 1.0 - clamp(r,0.0,1.0);
}

vec3 getNormal(vec3 point) {
  float dist = sceneDE(point);
  vec2 eps = vec2(EPSILON, 0.0);

  vec3 normal = dist - vec3(sceneDE(point - eps.xyy),
                            sceneDE(point - eps.yxy),
                            sceneDE(point - eps.yyx));
  return normalize(normal);
}

// Return: vec4(last distance step, minimum distance step, total distance, ray marching steps)
vec4 rayMarch(Ray ray) {

  float rayMarchSteps = 0.0;
  float distStep = MIN_DIST;
  // float minDistStep = FLT_MAX;
  float minDistStep = 1.0;
  float totalDist = 0.0;
  vec3 currPoint = ray.origin;

  for(; rayMarchSteps < MAX_STEPS; rayMarchSteps += 1.0) {
    distStep = sceneDE(currPoint);

    totalDist += distStep;
    minDistStep = min(minDistStep, distStep);

    if(totalDist > MAX_DIST || distStep < MIN_DIST) break;

    currPoint += distStep * ray.direction;
  }

  return vec4(distStep, minDistStep, totalDist, rayMarchSteps);
}

// Directional light ray march
float rayMarchDirLight(Ray shadowRay) {

  float shadowCoef = 1.0;
  float rayMarchSteps = 0.0;
  float maxSteps = MAX_STEPS / 2.0; // Optimization
  float distStep = MIN_DIST;
  float totalDist = 0.0;
  vec3 currPoint = shadowRay.origin;

  for(; rayMarchSteps < maxSteps; rayMarchSteps += 1.0) {
    distStep = sceneDE(currPoint);

    totalDist += distStep;

    // Simple soft shadows
    shadowCoef = min(shadowCoef, settings.shadowSharpness * distStep / totalDist);

    // Improved technique
    // float y = distStep * distStep / (2.0*previusDistStep);
    // float d = sqrt(distStep*distStep-y*y);
    // shadowCoef = min( shadowCoef, settings.shadow_sharpness * d / max(0.0,totalDist-y) );

    if(distStep < MIN_DIST) return 0.0; // Hit
    if(totalDist > MAX_DIST) break; // No hit

    currPoint += distStep * shadowRay.direction;
  }

  return shadowCoef;
}

vec3 getShadingDirectional(vec3 point, vec3 normal, Ray viewRay, vec3 dirLight) {
  vec3 origin = point + normal * MIN_DIST * 2.0;

  // Diffuse
  dirLight = normalize(vec3(settings.view * vec4(dirLight, 0.0))); // Rotate with camera
  float diffuse = dot(normal, -dirLight);

  // Specular
  vec3 viewDir = normalize(vec3(viewRay.origin) - point);
  vec3 reflectedLightDir = reflect(dirLight, normal);
  float specular = pow(max(dot(viewDir, reflectedLightDir), 0.0), 64.0);

  Ray shadowRay = Ray(origin, -dirLight);
  float shadowCoef= rayMarchDirLight(shadowRay);

  return shadowCoef * (settings.dirLightColor * settings.dirIntensity * diffuse +
                       settings.specularColor * settings.specularIntensity * specular);
}

// Point light ray march
// float rayMarchPointLight(Ray shadowRay, float lightDist) {

//   float shadowCoef = 1.0;
//   float distStep = MIN_DIST;
//   float totalDist = MAX_DIST;
//   // float previusDistStep = 1e20;
//   vec3 currPoint = shadowRay.origin;

//   for(float totalDist=0.001; totalDist < lightDist; ) { // No point going further than light distance
//     distStep = sceneDE(currPoint);

//     totalDist += distStep;

//     // Simple soft shadows
//     shadowCoef = min(shadowCoef, settings.shadowSharpness * distStep / totalDist);

//     // Improved technique
//     // float y = distStep * distStep / (2.0*previusDistStep);
//     // float d = sqrt(distStep*distStep-y*y);
//     // shadowCoef = min( shadowCoef, settings.shadow_sharpness * d / max(0.0,dist-y) );

//     if(distStep < MIN_DIST) return 0.03; // Hit
//     if(totalDist > MAX_DIST) break; // No hit

//     currPoint += distStep * shadowRay.direction;
//   }

//   return shadowCoef;
// }

// Point light shading
// vec3 getShadingPoint(vec3 point, vec3 normal) {
//   float pointShadeCoef = 1.0;
//   vec3 origin = point + normal * MIN_DIST * 2.0;

//   // Point light
//   // TODO: extened to multiple
//   vec3 lightDir = normalize(settings.pointLightPos - point);
//   // vec3 lightDir = normalize(pointLightPos - point);
//   pointShadeCoef *= dot(normal, -dirLight);

//   Ray shadowRay = Ray(origin, lightDir);
//   float lightDist = length(settings.pointLightPos - point);
//   pointShadeCoef *= rayMarchPointLight(shadowRay, lightDist);

//   float attenuation = (1.0 / (1.0 + 0.027*lightDist + 0.028*lightDist*lightDist));
//   pointShadeCoef *= attenuation;

//   return settings.pointLightColor * pointShadeCoef  * settings.pointIntensity;
// }


vec3 scene(Ray ray) {
  // Cast ray
  vec4 rayCast = rayMarch(ray);
  float lastDistStep = rayCast.x;
  float minDistStep = rayCast.y;
  float totalDist = rayCast.z;
  float rayMarchSteps = rayCast.w;

  vec3 col = vec3(0.0);
  float fogAmount = clamp(1.0 - exp(-(totalDist - settings.fogStartDist) * (1.0/settings.fogCurve)), 0.0, 1.0);

  // Hit
  if(lastDistStep < MIN_DIST) {
    vec3 hitPoint = ray.origin + totalDist * ray.direction;
    // Surface normal
    vec3 normal = getNormal(hitPoint);

    // Shading
    if(settings.dirLightShading == 1.0) {
      col += getShadingDirectional(hitPoint, normal, ray, dirLight1);
      col += getShadingDirectional(hitPoint, normal, ray, dirLight2);
      col += getShadingDirectional(hitPoint, normal, ray, dirLight3);
      col += getShadingDirectional(hitPoint, normal, ray, dirLight4);
    // col += getShadingPoint(hitPoint, normal);
    }

    // Ambient occlusion
    col += settings.AOColor * settings.AOIntensity * CalcAO(hitPoint, normal);

    // Fog
    col = mix(col, settings.backgroundColor, fogAmount);


  } else { // Miss or run out of ray march steps

    // Background color
    col += settings.backgroundColor;

    // Fog
    col = mix(col, settings.backgroundColor, fogAmount);
  }

  // vec3 glow = (1.0 - minDistStep) * (1.0 - minDistStep) * settings.glowColor;
  // vec3 glow = smoothstep(0.0, 1.0, -minDistStep + 0.2) * 2.0 * settings.glowColor; 
  vec3 glow = settings.glowColor * settings.glowIntensity * rayMarchSteps / MAX_STEPS;

  return col + glow;
}


void main() {
  vec3 col = vec3(0.0); // Accumulate color

  // Normalized screen cordinates -> screen ray intersection cordinates
  vec2 uv = inUV * 2.0 - 1.0;

  for(int i = 0; i < ANTIALISING_SAMPLES; i++) {
    for(int j = 0; j < ANTIALISING_SAMPLES; j++) {

      vec2 delta = vec2(i, j) / (ANTIALISING_SAMPLES);
      uv += delta / vec2(settings.viewportWidth, settings.viewportHeight); // Need screen resolution

      Ray viewRay = generateViewRay(vec3(settings.cameraOrigin), uv);

      col += scene(viewRay);
    }
  }

  // Accumulate
  col /= ANTIALISING_SAMPLES * ANTIALISING_SAMPLES;

  // Effects
  float VIGNETTE_STRENGTH = 0.5;
  float vignette = 1.0 - VIGNETTE_STRENGTH * length(uv);
  col *= vignette;

  // col = smoothstep(0.0, 1.0, col); // contrast
  col = pow(col, vec3(.4545));	// gamma correction
  outFragColor = vec4(col, 1.0);
}
