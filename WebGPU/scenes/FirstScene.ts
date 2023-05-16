import SceneBase from "./SceneBase";
import Camera from "@WebGPU/camera/Camera";
import { mat4, vec3 } from "gl-matrix";
import Box from "@WebGPU/objects/Box";
import FirstPipeline from "@WebGPU/renderPipeline/FirstPipeline";
import Objects from "@WebGPU/objects/Objects";

export default class FirstScene extends SceneBase {
    // texture: HTMLImageElement = null;
    // box: Box;

    constructor(ctx: GPUCanvasContext, device: GPUDevice) {
        super(ctx, device, Math.PI / 4, 0.1, 100);
        this.renderPipeline = new FirstPipeline(device);
        this.camera = new Camera(vec3.fromValues(0, 0, 5), 10, 10);
        // this.box = new Box(device);
    }

    execCommand(): GPUCommandBuffer[] {
        const clearColor: GPUColor = { r: 0.0, g: 0.5, b: 1.0, a: 1.0 };
        const renderPassDescriptor: GPURenderPassDescriptor = {
            colorAttachments: [
                {
                    clearValue: clearColor,
                    loadOp: "clear",
                    storeOp: "store",
                    view: this.ctx.getCurrentTexture().createView(),
                },
            ],
        };
        return [this.renderPass(renderPassDescriptor, this.renderPassCallback.bind(this))];
    }

    renderPassCallback(passEncoder: GPURenderPassEncoder): void {
        const vertices = new Float32Array([
            0.0, 0.6, 0, 1, 1, 0, 0, 1, -0.5, -0.6, 0, 1, 0, 1, 0, 1, 0.5, -0.6, 0, 1, 0, 0, 1, 1,
        ]);
        const renderPipeline = this.renderPipeline.initRenderPipeline();
        passEncoder.setPipeline(renderPipeline);
        passEncoder.setVertexBuffer(
            0,
            Objects.initBufferData(vertices, 24, GPUBufferUsage.VERTEX | GPUBufferUsage.COPY_DST, this.device)
        );
        passEncoder.draw(3);
        passEncoder.end();
    }
}
