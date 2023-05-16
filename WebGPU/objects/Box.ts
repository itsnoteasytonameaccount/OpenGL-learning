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
    vertex: GPUBuffer;
    texCroods: GPUBuffer;
    indices: GPUBuffer;
    constructor(device: GPUDevice, drawElements: boolean = true) {
        super(device);
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
     * @returns {GPUBuffer} 顶点纹理坐标
     */
    private getTextureCroods(): GPUBuffer {
        const texCroods = new Float32Array(arrTexCroods);
        return Objects.initBufferData(
            texCroods,
            arrTexCroods.length,
            GPUBufferUsage.VERTEX | GPUBufferUsage.COPY_DST,
            this.device
        );
    }

    /**
     * 获取顶点数据
     * @returns {GPUBuffer} 顶点数据数组
     */
    private getVertex(): GPUBuffer {
        const vertex = new Float32Array(arrVertex);
        return Objects.initBufferData(
            vertex,
            arrVertex.length,
            GPUBufferUsage.COPY_DST | GPUBufferUsage.VERTEX,
            this.device
        );
    }

    /**
     * 获取顶点索引数据
     * @returns {GPUBuffer} 顶点索引数据数组
     */
    private getIndices(): GPUBuffer {
        const indices = new Uint16Array(arrIndices);
        return Objects.initBufferData(
            indices,
            arrIndices.length,
            GPUBufferUsage.INDEX | GPUBufferUsage.COPY_DST,
            this.device
        );
    }

    /**
     * 顶过drawArray形式绘制时，顶点坐标列表
     *
     * @returns {GPUBuffer} 顶点缓冲数据
     */
    private getDrawArraysVertex(): GPUBuffer {
        const vertices = arrIndices.map((index) => arrVertex[index]);
        const data = new Float32Array(vertices);
        return Objects.initBufferData(
            data,
            vertices.length,
            GPUBufferUsage.COPY_DST | GPUBufferUsage.VERTEX,
            this.device
        );
    }

    /**
     * 顶过drawArray形式绘制时，顶点纹理坐标数据
     *
     * @returns {GPUBuffer} 顶点缓冲数据
     */
    private getDrawArraysTexCroods(): GPUBuffer {
        const arr: number[] = new Array(6).fill(arrTexCroods.slice(0, 12)).flat(1);
        const texCroods = new Float32Array(arr);
        return Objects.initBufferData(
            texCroods,
            arr.length,
            GPUBufferUsage.COPY_DST | GPUBufferUsage.VERTEX,
            this.device
        );
    }
}
