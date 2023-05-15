type WebGLContext = WebGLRenderingContext | WebGL2RenderingContext;
type GLShader = WebGLShader | null;
type ShaderType = WebGLRenderingContextBase["VERTEX_SHADER"] | WebGLRenderingContextBase["FRAGMENT_SHADER"];
type BufferType = WebGLRenderingContextBase["ARRAY_BUFFER"] | WebGLRenderingContextBase["ELEMENT_ARRAY_BUFFER"];
type WebGLTextureId =
    | WebGLRenderingContextBase["TEXTURE0"]
    | WebGLRenderingContextBase["TEXTURE1"]
    | WebGLRenderingContextBase["TEXTURE2"]
    | WebGLRenderingContextBase["TEXTURE3"]
    | WebGLRenderingContextBase["TEXTURE4"]
    | WebGLRenderingContextBase["TEXTURE5"]
    | WebGLRenderingContextBase["TEXTURE6"]
    | WebGLRenderingContextBase["TEXTURE7"]
    | WebGLRenderingContextBase["TEXTURE8"]
    | WebGLRenderingContextBase["TEXTURE9"]
    | WebGLRenderingContextBase["TEXTURE10"]
    | WebGLRenderingContextBase["TEXTURE11"]
    | WebGLRenderingContextBase["TEXTURE12"];
