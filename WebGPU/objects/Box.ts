import Texture from "@WebGPU/tools/Texture";
import path from "@WebGPU/resources/texture/container2.png";
import Objects from "./Objects";

const arrIndices = [
    0, 1, 2, 0, 2, 3, 0, 1, 4, 1, 4, 5, 0, 3, 4, 3, 4, 7, 2, 6, 1, 6, 1, 5, 2, 6, 3, 6, 3, 7, 4, 5, 6, 4, 6, 7,
];

const arrVertex = [-1, 1, 1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, -1, 1, 1, -1, 1, -1, -1, -1, -1, -1];

const arrTexCroods = [
    // 这个数据有点问题
    0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0,
];

export default class Box extends Objects {
    vertexBuffer: GPUBuffer;
    texture: GPUTexture = null;

    constructor(device: GPUDevice) {
        super(device);
        this.vertexBuffer = this.initVertexBuffer();
        Texture.createTexture(path, device).then((texture) => {
            console.log("texture loaded");
            this.texture = texture;
        });
    }

    private initVertexBuffer(): GPUBuffer {
        const faceTexture = arrTexCroods.slice(0, 12);
        const vertices: number[] = arrIndices
            .map((ind, index) => {
                const texIndex = index % 6;
                const result = [
                    ...arrVertex.slice(ind * 3, (ind + 1) * 3),
                    1.0,
                    ...faceTexture.slice(texIndex * 2, (texIndex + 1) * 2),
                    0,
                    1.0,
                ];
                console.log(result);
                return result;
            })
            .flat(1);
        return Objects.initBufferData(
            new Float32Array(vertices),
            vertices.length,
            GPUBufferUsage.COPY_DST | GPUBufferUsage.VERTEX,
            this.device
        );
    }

    getIndicesSize(): number {
        return arrIndices.length;
    }
}
