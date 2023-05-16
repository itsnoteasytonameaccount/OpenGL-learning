import code from "@GPUShader/firstShader.wgsl?raw";
import PipelineBase from "./PipelineBase";

export default class FirstPipeline extends PipelineBase {
    constructor(device: GPUDevice) {
        super(device, code);
    }

    createRenderPipelineDescriptor(vertexBuffers: GPUVertexBufferLayout[]): GPURenderPipelineDescriptor {
        const shaderModule = this.shaderModule;
        return {
            vertex: {
                module: shaderModule,
                entryPoint: "vertex_main",
                buffers: vertexBuffers,
            },
            fragment: {
                module: shaderModule,
                entryPoint: "fragment_main",
                targets: [
                    {
                        format: navigator.gpu.getPreferredCanvasFormat(),
                    },
                ],
            },
            primitive: {
                topology: "triangle-list",
            },
            layout: "auto",
        };
    }

    createVertexBuffers(): GPUVertexBufferLayout[] {
        return [
            {
                attributes: [
                    {
                        shaderLocation: 0, // 位置
                        offset: 0,
                        format: "float32x4",
                    },
                    {
                        shaderLocation: 1, // 颜色
                        offset: 16,
                        format: "float32x4",
                    },
                ],
                arrayStride: 32,
                stepMode: "vertex",
            },
        ];
    }
}
