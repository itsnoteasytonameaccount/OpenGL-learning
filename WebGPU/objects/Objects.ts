import { glMatrix, mat4, vec3 } from "gl-matrix";

export default abstract class Objects {
    device: GPUDevice;
    modelMatrix: mat4 = mat4.create();

    constructor(device: GPUDevice) {
        this.device = device;
    }

    /**
     * 创建缓存并填入数据
     *
     * @param data 缓存数据
     * @param type 缓存类型
     * @param gl webgl实例
     * @returns 生成的缓存
     */
    static initBufferData(data: ArrayBuffer, size: number, type: GPUBufferUsageFlags, device: GPUDevice): GPUBuffer {
        const buffer: GPUBuffer = device.createBuffer({
            size: data.byteLength,
            usage: type,
        });
        device.queue.writeBuffer(buffer, 0, data, 0, size);
        return buffer;
    }

    /**
     * 创建缓存并填入数据
     *
     * @param datas 缓存数据列表
     * @param sizes 数据大小列表
     * @param type 缓存类型
     * @param gl webgl实例
     * @returns 生成的缓存
     */
    static initBufferDatas(
        datas: ArrayBuffer[],
        sizes: number[],
        type: GPUBufferUsageFlags,
        device: GPUDevice
    ): GPUBuffer {
        const total = datas.reduce((sum, data) => sum + data.byteLength, 0);
        const buffer: GPUBuffer = device.createBuffer({
            size: total,
            usage: type,
        });
        let used = 0;
        datas.forEach((data, index) => {
            device.queue.writeBuffer(buffer, used, data, 0, sizes[index]);
            used += data.byteLength;
        });
        return buffer;
    }

    static initEmptyBuffer(size: number, type: GPUBufferUsageFlags, device: GPUDevice): GPUBuffer {
        return device.createBuffer({
            size,
            usage: type,
        });
    }

    static writeBufferDatas(buffer: GPUBuffer, datas: ArrayBuffer[], sizes: number[], device: GPUDevice) {
        let used = 0;
        datas.forEach((data, index) => {
            device.queue.writeBuffer(buffer, used, data, 0, sizes[index]);
            used += data.byteLength;
        });
    }

    transform(matrix: mat4) {
        mat4.mul(this.modelMatrix, matrix, this.modelMatrix);
    }

    clear() {
        this.modelMatrix = mat4.create();
    }

    translate(x: number, y: number, z: number) {
        const translateMatrix = mat4.create();
        mat4.translate(translateMatrix, translateMatrix, vec3.fromValues(x, y, z));
        mat4.mul(this.modelMatrix, translateMatrix, this.modelMatrix);
    }

    rotate(angle: number, axis: vec3 = vec3.fromValues(0, 1.0, 0)) {
        const radians = glMatrix.toRadian(angle);
        const rotateMatrix = mat4.create();
        mat4.rotate(rotateMatrix, rotateMatrix, radians, axis);
        mat4.mul(this.modelMatrix, rotateMatrix, this.modelMatrix);
    }

    selfRotate(angle: number, axis: vec3 = vec3.fromValues(0, 1.0, 0)) {
        const radians = glMatrix.toRadian(angle);
        mat4.rotate(this.modelMatrix, this.modelMatrix, radians, axis);
    }

    getModelMaxtrix(): mat4 {
        return this.modelMatrix;
    }

    abstract getIndicesSize(): number;
}
