import Camera from "@WebGPU/camera/Camera";
import { throttle } from "@WebGPU/tools/utils";
import PipelineBase from "@WebGPU/renderPipeline/PipelineBase";
import { mat4 } from "gl-matrix";

export default abstract class SceneBase {
    camera: Camera;
    renderPipeline: PipelineBase;
    perspectiveMatrix: mat4;
    ctx: GPUCanvasContext;
    device: GPUDevice;

    constructor(ctx: GPUCanvasContext, device: GPUDevice, viewAngle: number, near: number, far: number) {
        this.ctx = ctx;
        this.device = device;
        this.perspectiveMatrix = this.getProjectionMatrix(viewAngle, near, far);
        this.ctx.canvas.addEventListener("mousemove", throttle(this.handleMouseMove, 50).bind(this));
    }

    getProjectionMatrix(viewAngle: number, near: number, far: number): mat4 {
        const gl = this.ctx;
        const projectionMatrix = mat4.create();
        mat4.perspective(
            projectionMatrix, // 指针
            viewAngle, // 视角
            gl.canvas.width / gl.canvas.height, //宽高比
            near, // 近景
            far // 远景
        );
        return projectionMatrix;
    }

    handleMouseMove(e: MouseEvent) {
        const { offsetX, offsetY } = e;
        const { clientHeight: height, clientWidth: width } = <Element>this.ctx.canvas;
        const x = offsetX - width / 2;
        const y = offsetY - height / 2;
        const yaw = (x / width) * 2 * (45 / 2);
        const pitch = (-y / height) * 2 * (45 / 2);
        this.camera.setYawPitch(yaw, pitch);
    }

    abstract execCommand(): Array<GPUCommandBuffer>;

    protected renderPass(descriptor: GPURenderPassDescriptor, callback: RenderPassCallback): GPUCommandBuffer {
        const commandEncoder = this.device.createCommandEncoder();
        const renderPass = commandEncoder.beginRenderPass(descriptor);
        callback(renderPass);
        return commandEncoder.finish();
    }

    draw(): void {
        const commands: Array<GPUCommandBuffer> = this.execCommand();
        this.device.queue.submit(commands);
    }
}
