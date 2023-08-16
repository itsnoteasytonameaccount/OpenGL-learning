import SceneBase from "./SceneBase";
import Camera from "@WebGL/camera/Camera";
import { mat3, mat4, vec3 } from "gl-matrix";
import Box from "@WebGL/polygon/Box";
import textureImage from "@texture/container2.png";
import cubeMapsImage from "@texture/skybox-moutain.png";
import SkyBoxProgram from "@WebGL/program/SkyBoxProgram";
import SkyBox from "@WebGL/polygon/SkyBox";
import PhoneLightProgram from "@WebGL/program/PhoneLightProgram";

export default class PhoneLightScene extends SceneBase {
    box: Box;
    background: SkyBox;
    backgroundProgram: WebGLProgram;

    constructor(ctx: WebGLContext) {
        super(ctx, Math.PI / 4, 0.1, 100);
        this.program = new PhoneLightProgram(ctx);
        this.backgroundProgram = new SkyBoxProgram(ctx);
        this.camera = new Camera(vec3.fromValues(0, 0, 5), 0, 0);
        this.box = new Box(ctx, textureImage, false);
        this.background = new SkyBox(ctx, cubeMapsImage);
    }

    draw(): void {
        const box = this.box;
        const program = <PhoneLightProgram>this.program;
        const backgroundProgram = <SkyBoxProgram>this.backgroundProgram;
        const { lightColor, lightPos } = this.collectFormValue();
        this.reset();
        box.clear();
        box.translate(0, 0, -5);
        // box.selfRotate(Math.floor(Date.now() / 10) % 360, vec3.fromValues(0, 1, 0));
        program.use();

        const projection = program.getUniformLocation("projection");
        const model = program.getUniformLocation("model");
        const view = program.getUniformLocation("view");
        const normalModel = program.getUniformLocation("normalModel");
        const lightPosLocation = program.getUniformLocation("lightPos");
        const lightColorLocation = program.getUniformLocation("lightColor");
        const normalModelMatrix = mat3.create();
        mat3.fromMat4(normalModelMatrix, mat4.mul(mat4.create(), this.camera.getViewMatrix(), box.getModelMaxtrix()));
        mat3.transpose(normalModelMatrix, mat3.invert(mat3.create(), normalModelMatrix));
        const gl = this.ctx;
        gl.uniformMatrix4fv(projection, false, this.perspectiveMatrix);
        gl.uniformMatrix4fv(model, false, box.getModelMaxtrix());
        gl.uniformMatrix4fv(view, false, this.camera.getViewMatrix());
        gl.uniformMatrix3fv(normalModel, false, normalModelMatrix);
        gl.uniform3f(lightPosLocation, lightPos.x, lightPos.y, lightPos.z);
        gl.uniform3f(lightColorLocation, lightColor.r, lightColor.g, lightColor.b);
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

    collectFormValue(): any {
        const result = {};
        const fields = document.querySelectorAll("#webgl-config input");
        Array.prototype.forEach.call(fields, this.setField.bind(null, result));
        return result;
    }

    setField(obj: any, input: HTMLInputElement) {
        const names = input.name.split(".");
        let child = obj;
        while (names.length > 1) {
            const key = names.shift();
            const parent = child;
            child = child[key] || {};
            parent[key] = child;
        }
        if (names.length === 1) {
            child[names.shift()] = Number(input.value);
        }
    }
}
