export default abstract class ProgramBase {
    ctx: WebGLContext;
    glVersion: number;
    program: WebGLProgram;

    constructor(ctx: WebGLContext) {
        this.ctx = ctx;
        if (ctx instanceof WebGL2RenderingContext) {
            this.glVersion = 2;
        } else {
            this.glVersion = 1;
        }
        this.program = this.initProgram();
    }

    static compileShader(code: string, type: ShaderType, gl: WebGLContext): WebGLShader {
        const shader = gl.createShader(type);
        if (!shader) {
            console.error("create shader error");
            throw new Error("create shader error");
        }
        gl.shaderSource(shader, code);
        gl.compileShader(shader);
        if (gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
            return shader;
        } else {
            console.error("compile shader error: ", gl.getShaderInfoLog(shader));
            throw new Error("compile shader error: " + gl.getShaderInfoLog(shader));
        }
    }

    initProgram(): WebGLProgram {
        const ctx = this.ctx;
        const program: WebGLProgram | null = ctx.createProgram();
        if (!program) {
            throw new Error("Create program failed!");
        }
        const fragmentShader: GLShader = this.getFragmentShader();
        const vertexShader: GLShader = this.getVertexShader();
        if (fragmentShader) {
            ctx.attachShader(program, fragmentShader);
        }
        if (vertexShader) {
            ctx.attachShader(program, vertexShader);
        }
        ctx.linkProgram(program);
        ctx.deleteShader(fragmentShader);
        ctx.deleteShader(vertexShader);
        if (ctx.getProgramParameter(program, ctx.LINK_STATUS)) {
            return program;
        } else {
            throw new Error("Link program failed!");
        }
    }

    enableVertexArray(buffer: WebGLBuffer, location: number, size: number): void {
        const gl = this.ctx;
        gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
        gl.vertexAttribPointer(location, size, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(location);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);
    }

    use() {
        this.ctx.useProgram(this.program);
    }

    abstract getVertexShader(): GLShader;

    abstract getFragmentShader(): GLShader;

    abstract setParameters(attributes: AttributeDescriptor[], uniforms: UnifromDescriptor[]);
}
