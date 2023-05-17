struct VertexOut {
  @builtin(position) position : vec4f,
  @location(0) fragUV : vec2f
}

struct Matrixs {
  modelMatrix : mat4x4f,
  viewMatrix : mat4x4f,
  projectionMatrix: mat4x4f
}

@group(0) @binding(0) var<uniform> matrixs: Matrixs;

@vertex
fn vertex_main(@location(0) position: vec4f,
               @location(1) color: vec4f) -> VertexOut
{
  var output : VertexOut;
  var verdata = vec4f(matrixs.projectionMatrix * matrixs.viewMatrix * matrixs.modelMatrix * position);
  output.position = verdata;
  output.fragUV = vec2f(color.x, color.y);
  return output;
}

@group(0) @binding(1) var mySampler: sampler;
@group(0) @binding(2) var myTexture: texture_2d<f32>;

@fragment
fn fragment_main(fragData: VertexOut) -> @location(0) vec4f
{
  return textureSample(myTexture, mySampler, fragData.fragUV);
}