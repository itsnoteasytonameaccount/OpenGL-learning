export default abstract class PipelineBase {
    device: GPUDevice;
    shaderModule: GPUShaderModule;

    constructor(device: GPUDevice, code: string) {
        this.device = device;
        this.shaderModule = this.createShader(code);
    }

    private createShader(code: string): GPUShaderModule {
        return this.device.createShaderModule({
            code,
        });
    }

    abstract createRenderPipelineDescriptor(vertexBuffers: Array<GPUVertexBufferLayout>): GPURenderPipelineDescriptor;

    abstract createVertexBuffers(): Array<GPUVertexBufferLayout>;

    initRenderPipeline(): GPURenderPipeline {
        const vertexBuffers = this.createVertexBuffers();
        return this.device.createRenderPipeline(this.createRenderPipelineDescriptor(vertexBuffers));
    }
}
