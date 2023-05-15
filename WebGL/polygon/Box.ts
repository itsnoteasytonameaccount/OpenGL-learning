import Objects from "./Objects";

const arrIndices = [
    0, 1, 2, 0, 2, 3, 0, 1, 4, 1, 4, 5, 0, 3, 4, 3, 4, 7, 2, 6, 1, 6, 1, 5, 2, 6, 3, 6, 3, 7, 4, 5, 6, 4, 6, 7,
];

const arrVertex = [-1, 1, 1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, -1, 1, 1, -1, 1, -1, -1, -1, -1, -1];

const arrTexCroods = [
    // 这个数据有点问题
    0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0,
];

export default class Box extends Objects {
    vertex: WebGLBuffer;
    texCroods: WebGLBuffer;
    indices: WebGLBuffer;
    constructor(ctx: WebGLContext, drawElements: boolean = true) {
        super(ctx);
        if (drawElements) {
            this.createDataBuffer();
        } else {
            this.createDrawArraysDataBuffer();
        }
    }

    createDataBuffer(): void {
        this.vertex = this.getVertex();
        this.indices = this.getIndices();
        this.texCroods = this.getTextureCroods();
    }

    createDrawArraysDataBuffer(): void {
        this.vertex = this.getDrawArraysVertex();
        this.texCroods = this.getDrawArraysTexCroods();
    }

    /**
     * 顶点纹理坐标
     *
     * @returns {WebGLBuffer} 顶点纹理坐标
     */
    private getTextureCroods(): WebGLBuffer {
        const gl = this.ctx;
        const texCroods = new Float32Array(arrTexCroods);
        return Objects.initBufferData(texCroods, gl.ARRAY_BUFFER, gl);
    }

    /**
     * 获取顶点数据
     * @returns {WebGLBuffer} 顶点数据数组
     */
    private getVertex(): WebGLBuffer {
        const gl = this.ctx;
        const vertex = new Float32Array(arrVertex);
        return Objects.initBufferData(vertex, gl.ARRAY_BUFFER, gl);
    }

    /**
     * 获取顶点索引数据
     * @returns {WebGLBuffer} 顶点索引数据数组
     */
    private getIndices(): WebGLBuffer {
        const gl = this.ctx;
        const indices = new Uint16Array(arrIndices);
        return Objects.initBufferData(indices, gl.ELEMENT_ARRAY_BUFFER, gl);
    }

    /**
     * 顶过drawArray形式绘制时，顶点坐标列表
     *
     * @returns {WebGLBuffer} 顶点缓冲数据
     */
    private getDrawArraysVertex(): WebGLBuffer {
        const gl = this.ctx;
        const vertex = new Float32Array(arrIndices.map((index) => arrVertex[index]));
        return Objects.initBufferData(vertex, gl.ARRAY_BUFFER, gl);
    }

    /**
     * 顶过drawArray形式绘制时，顶点纹理坐标数据
     *
     * @returns {WebGLBuffer} 顶点缓冲数据
     */
    private getDrawArraysTexCroods(): WebGLBuffer {
        const gl = this.ctx;
        const arr: number[] = new Array(6).fill(arrTexCroods.slice(0, 12)).flat(1);
        const texCroods = new Float32Array(arr);
        return Objects.initBufferData(texCroods, gl.ARRAY_BUFFER, gl);
    }
}
