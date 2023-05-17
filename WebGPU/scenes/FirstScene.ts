import SceneBase from "./SceneBase";
import Camera from "@WebGPU/camera/Camera";
import { mat4, vec3 } from "gl-matrix";
import Box from "@WebGPU/objects/Box";
import FirstPipeline from "@WebGPU/renderPipeline/FirstPipeline";
import Objects from "@WebGPU/objects/Objects";

export default class FirstScene extends SceneBase {
    // texture: HTMLImageElement = null;
    box: Box;
    matrixBuffer: GPUBuffer;
    projection: ArrayBuffer;

    constructor(ctx: GPUCanvasContext, device: GPUDevice) {
        super(ctx, device, Math.PI / 4, 0.1, 100);
        this.renderPipeline = new FirstPipeline(device);
        this.camera = new Camera(vec3.fromValues(0, 0, 5), 10, 10);
        this.projection = new Float32Array(this.perspectiveMatrix);
        this.matrixBuffer = Objects.initEmptyBuffer(
            4 * 4 * 4 * 3,
            GPUBufferUsage.UNIFORM | GPUBufferUsage.COPY_DST,
            this.device
        );
        this.box = new Box(device);
    }

    execCommand(): GPUCommandBuffer[] {
        const clearColor: GPUColor = { r: 0.0, g: 0.5, b: 1.0, a: 1.0 };
        const canvas = this.ctx.canvas;
        const depthTexture = this.device.createTexture({
            size: [canvas.width, canvas.height],
            format: "depth24plus",
            usage: GPUTextureUsage.RENDER_ATTACHMENT,
        });
        const renderPassDescriptor: GPURenderPassDescriptor = {
            colorAttachments: [
                {
                    clearValue: clearColor,
                    loadOp: "clear",
                    storeOp: "store",
                    view: this.ctx.getCurrentTexture().createView(),
                },
            ],
            depthStencilAttachment: {
                view: depthTexture.createView(),
                depthClearValue: 1.0,
                depthLoadOp: "clear",
                depthStoreOp: "store",
            },
        };
        return [this.renderPass(renderPassDescriptor, this.renderPassCallback.bind(this))];
    }

    renderPassCallback(passEncoder: GPURenderPassEncoder): void {
        const renderPipeline = this.renderPipeline.initRenderPipeline();
        const device: GPUDevice = this.device;
        passEncoder.setPipeline(renderPipeline);
        this.box.clear();
        this.box.translate(0, 0, -10);
        this.box.selfRotate(Math.floor(Date.now() / 10) % 360, vec3.fromValues(0, 1, 1));
        passEncoder.setVertexBuffer(0, this.box.vertexBuffer);
        const canvas = this.ctx.canvas;
        const texture =
            this.box.texture === null
                ? this.device.createTexture({
                      size: [canvas.width, canvas.height],
                      format: "rgba8unorm",
                      usage:
                          GPUTextureUsage.TEXTURE_BINDING |
                          GPUTextureUsage.COPY_DST |
                          GPUTextureUsage.RENDER_ATTACHMENT,
                  })
                : this.box.texture;
        Objects.writeBufferDatas(
            this.matrixBuffer,
            [new Float32Array(this.box.modelMatrix), new Float32Array(this.camera.getViewMatrix()), this.projection],
            [16, 16, 16],
            device
        );
        passEncoder.setBindGroup(
            0,
            (<FirstPipeline>this.renderPipeline).getBindGroup(this.matrixBuffer, texture, renderPipeline)
        );
        passEncoder.draw(36);
        passEncoder.end();
    }
}
