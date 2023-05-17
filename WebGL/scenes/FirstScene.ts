import BasicProgram from "@WebGL/program/BasicProgram";
import SceneBase from "./SceneBase";
import Camera from "@WebGL/camera/Camera";
import { mat4, vec3 } from "gl-matrix";
import Box from "@WebGL/polygon/Box";

export default class FirstScene extends SceneBase {
    box: Box;

    constructor(ctx: WebGLContext) {
        super(ctx, Math.PI / 4, 0.1, 100);
        this.program = new BasicProgram(ctx);
        this.camera = new Camera(vec3.fromValues(0, 0, 5), 10, 10);
        this.box = new Box(ctx);
    }
    draw(): void {
        const gl = this.ctx;
        const box = this.box;
        const program = <BasicProgram>this.program;
        this.reset();
        box.clear();
        box.translate(0, 0, -5);
        box.selfRotate(Math.floor(Date.now() / 10) % 360, vec3.fromValues(0, 1, 0));
        program.use();
        program.fillUniformProjectionMatrix(this.perspectiveMatrix);
        program.fillUniformModelViewMatrix(mat4.mul(mat4.create(), this.camera.getViewMatrix(), box.getModelMaxtrix()));
        box.draw(program);
        gl.drawElements(gl.TRIANGLES, box.getIndicesSize(), gl.UNSIGNED_SHORT, 0);
    }
}
