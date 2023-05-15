import Camera from "@WebGL/camera/Camera";
import ProgramBase from "@WebGL/program/ProgramBase";
import { throttle } from "@WebGL/tools/utils";
import { mat4 } from "gl-matrix";

export default abstract class SceneBase {
    camera: Camera;
    program: ProgramBase;
    perspectiveMatrix: mat4;
    ctx: WebGLContext;

    constructor(ctx: WebGLContext, viewAngle: number, near: number, far: number) {
        this.ctx = ctx;
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

    reset(): void {
        const gl = this.ctx;
        // 设置清除用的颜色为黑
        gl.clearColor(0, 0, 0, 1);
        // 设置清除深度为1
        gl.clearDepth(1);
        // 近处遮住远处
        gl.enable(gl.DEPTH_TEST); // Enable depth testing
        gl.depthFunc(gl.LEQUAL); // Near things obscure far things
        // 用预设颜色清除，清除预设深度
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    }

    abstract draw(): void;
}
