import { SIZEOF_FLOAT32 } from "@WebGL/definition/constant";

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

    /**
     * 编译着色器
     *
     * @param code 着色器代码
     * @param type 着色器类型
     * @param gl WebGL上下文
     * @returns 编译好的着色器对象
     * @throws {Error} 创建着色器对象失败或者编译失败
     */
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
            throw new Error("Link program failed!" + ctx.getProgramInfoLog(program));
        }
    }

    /**
     * 设置变量的数组缓冲指针
     *
     * @param buffer 数组缓冲
     * @param location 要设置的变量在着色器中的布局位置
     * @param size 取指针时的数据的长度，如果数据是整形，则表示循环的每一次取几个整数
     * @param stride 指针每次加一跨越的数组缓冲中的长度
     * @param offset 要填入的数据在数据组内的偏移量
     */
    enableVertexArray(
        buffer: WebGLBuffer,
        location: number,
        size: number,
        stride: number = 0,
        offset: number = 0
    ): void {
        const gl = this.ctx;
        gl.bindBuffer(gl.ARRAY_BUFFER, buffer);
        gl.vertexAttribPointer(location, size, gl.FLOAT, false, stride * SIZEOF_FLOAT32, offset * SIZEOF_FLOAT32);
        gl.enableVertexAttribArray(location);
        gl.bindBuffer(gl.ARRAY_BUFFER, null);
    }

    /**
     * 获取全局变量（通过uniform声明）布局位置
     *
     * @param name uniform变量名
     * @returns uniform变量的布局位置
     */
    getUniformLocation(name: string): WebGLUniformLocation {
        const gl = this.ctx;
        return gl.getUniformLocation(this.program, name);
    }

    /**
     * 获取输入属性（通过in声明）的布局位置
     *
     * @param name 输入属性变量名
     * @returns 输入属性布局位置
     */
    getAttributeLocation(name: string): number {
        return this.ctx.getAttribLocation(this.program, name);
    }

    use() {
        this.ctx.useProgram(this.program);
    }

    abstract getVertexShader(): GLShader;

    abstract getFragmentShader(): GLShader;
}
