import code from "@GPUShader/firstShader.wgsl?raw";
import PipelineBase from "./PipelineBase";

export default class FirstPipeline extends PipelineBase {
    sampler: GPUSampler;
    constructor(device: GPUDevice) {
        super(device, code);
        this.sampler = this.device.createSampler({
            magFilter: "linear",
            minFilter: "linear",
        });
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
            depthStencil: {
                depthWriteEnabled: true,
                depthCompare: "less",
                format: "depth24plus",
            },
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

    getBindGroup(buffer: GPUBuffer, texture: GPUTexture, renderPipeline: GPURenderPipeline): GPUBindGroup {
        return this.device.createBindGroup({
            layout: renderPipeline.getBindGroupLayout(0),
            entries: [
                {
                    binding: 0,
                    resource: {
                        buffer,
                        offset: 0,
                        size: 4 * 16 * 3,
                    },
                },
                {
                    binding: 1,
                    resource: this.sampler,
                },
                {
                    binding: 2,
                    resource: texture.createView(),
                },
            ],
        });
    }
}
