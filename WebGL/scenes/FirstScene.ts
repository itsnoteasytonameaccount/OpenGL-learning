import BasicProgram from "@WebGL/program/BasicProgram";
import SceneBase from "./SceneBase";
import Camera from "@WebGL/camera/Camera";
import { mat4, vec3 } from "gl-matrix";
import Box from "@WebGL/polygon/Box";
import textureImage from "@texture/container2.png";
import Texture from "@WebGL/tools/Texture";

export default class FirstScene extends SceneBase {
    texture: HTMLImageElement = null;
    box: Box;

    constructor(ctx: WebGLContext) {
        super(ctx, Math.PI / 4, 0.1, 100);
        this.program = new BasicProgram(ctx);
        this.camera = new Camera(vec3.fromValues(0, 0, 5), 10, 10);
        Texture.loadImage(textureImage).then((image) => (this.texture = image));
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
        program.fillUniformViewMatrix(mat4.mul(mat4.create(), this.camera.getViewMatrix(), box.getModelMaxtrix()));
        if (this.texture) {
            Texture.load2DTexture(gl.TEXTURE0, this.texture, gl);
        }
        program.useSampler(0);
        program.bindTexturePointer(box.texCroods);
        program.bindVertexPointer(box.vertex);
        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, box.indices);
        gl.drawElements(gl.TRIANGLES, 36, gl.UNSIGNED_SHORT, 0);
    }
}
