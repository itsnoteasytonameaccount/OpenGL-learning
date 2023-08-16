import BasicProgram from "@WebGL/program/BasicProgram";
import SceneBase from "./SceneBase";
import Camera from "@WebGL/camera/Camera";
import { mat4, vec3 } from "gl-matrix";
import Box from "@WebGL/polygon/Box";
import textureImage from "@texture/container2.png";
import cubeMapsImage from "@texture/skybox-moutain.png";
import SkyBoxProgram from "@WebGL/program/SkyBoxProgram";
import SkyBox from "@WebGL/polygon/SkyBox";

export default class FirstScene extends SceneBase {
    box: Box;
    background: SkyBox;
    backgroundProgram: WebGLProgram;

    constructor(ctx: WebGLContext) {
        super(ctx, Math.PI / 4, 0.1, 100);
        this.program = new BasicProgram(ctx);
        this.backgroundProgram = new SkyBoxProgram(ctx);
        this.camera = new Camera(vec3.fromValues(0, 0, 5), 10, 10);
        this.box = new Box(ctx, textureImage, false);
        this.background = new SkyBox(ctx, cubeMapsImage);
    }

    draw(): void {
        const box = this.box;
        const program = <BasicProgram>this.program;
        const backgroundProgram = <SkyBoxProgram>this.backgroundProgram;
        this.reset();
        box.clear();
        box.translate(0, 0, -5);
        box.selfRotate(Math.floor(Date.now() / 10) % 360, vec3.fromValues(0, 1, 0));
        program.use();
        program.fillUniformProjectionMatrix(this.perspectiveMatrix);
        program.fillUniformModelViewMatrix(mat4.mul(mat4.create(), this.camera.getViewMatrix(), box.getModelMaxtrix()));
        box.draw(program);
        box.translate(1, 2, 0);
        program.fillUniformModelViewMatrix(mat4.mul(mat4.create(), this.camera.getViewMatrix(), box.getModelMaxtrix()));
        box.draw(program);

        const background = this.background;
        backgroundProgram.use();
        backgroundProgram.fillUniformProjectionMatrix(this.perspectiveMatrix);
        backgroundProgram.fillUniformModelViewMatrix(
            mat4.mul(mat4.create(), this.removeTranslate(this.camera.getViewMatrix()), background.getModelMaxtrix())
        );
        this.background.draw(backgroundProgram);
    }

    removeTranslate(matrix: mat4): mat4 {
        const result: mat4 = mat4.clone(matrix);
        result[12] = 0;
        result[13] = 0;
        result[14] = 0;
        return result;
    }
}
