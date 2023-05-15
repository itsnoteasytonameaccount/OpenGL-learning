import Objects from "./Objects";

export default class Box extends Objects {
    vertex: WebGLBuffer;
    texCroods: WebGLBuffer;
    indices: WebGLBuffer;
    constructor(ctx: WebGLContext) {
        super(ctx);
        this.createDataBuffer();
    }

    createDataBuffer(): void {
        this.vertex = this.getVertex();
        this.indices = this.getIndices();
        this.texCroods = this.getTextureCroods();
    }

    /**
     * 顶点纹理坐标
     *
     * @returns {Array<number>} 顶点纹理坐标
     */
    private getTextureCroods(): WebGLBuffer {
        const gl = this.ctx;
        const texCroods = new Float32Array([
            0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0,
        ]);
        return Objects.initBufferData(texCroods, gl.ARRAY_BUFFER, gl);
    }

    /**
     * 获取顶点数据
     * @returns {Array<number>} 顶点数据数组
     */
    private getVertex(): WebGLBuffer {
        const gl = this.ctx;
        const vertex = new Float32Array([
            -1, 1, 1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, -1, 1, 1, -1, 1, -1, -1, -1, -1, -1,
        ]);
        return Objects.initBufferData(vertex, gl.ARRAY_BUFFER, gl);
    }

    /**
     * 获取顶点索引数据
     * @returns {Array<number>} 顶点索引数据数组
     */
    private getIndices(): WebGLBuffer {
        const gl = this.ctx;
        const indices = new Uint16Array([
            0, 1, 2, 0, 2, 3, 0, 1, 4, 1, 4, 5, 0, 3, 4, 3, 4, 7, 2, 6, 1, 6, 1, 5, 2, 6, 3, 6, 3, 7, 4, 5, 6, 4, 6, 7,
        ]);
        return Objects.initBufferData(indices, gl.ELEMENT_ARRAY_BUFFER, gl);
    }
}
