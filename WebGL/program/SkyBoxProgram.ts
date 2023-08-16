import vsShader from "@WebGL/resources/shader/skybox/cubemaps.vs?raw";
import fsShader from "@WebGL/resources/shader/skybox/cubemaps.fs?raw";
import ProgramBase from "./ProgramBase";
import { mat4 } from "gl-matrix";

export default class SkyBoxProgram extends ProgramBase {
    projection: WebGLUniformLocation;
    view: WebGLUniformLocation;
    sampler: WebGLUniformLocation;

    constructor(ctx: WebGLContext) {
        super(ctx);
        this.initLocation();
    }

    initLocation() {
        const gl = this.ctx;
        const program = this.program;
        this.projection = gl.getUniformLocation(program, "projection");
        this.view = gl.getUniformLocation(program, "view");
        this.sampler = gl.getUniformLocation(program, "samplerTexture");
    }

    getVertexShader(): GLShader {
        return ProgramBase.compileShader(vsShader, this.ctx.VERTEX_SHADER, this.ctx);
    }

    getFragmentShader(): GLShader {
        return ProgramBase.compileShader(fsShader, this.ctx.FRAGMENT_SHADER, this.ctx);
    }

    fillUniformProjectionMatrix(projectionMatrix: mat4): void {
        this.ctx.uniformMatrix4fv(this.projection, false, projectionMatrix);
    }

    fillUniformModelViewMatrix(viewMatrix: mat4): void {
        this.ctx.uniformMatrix4fv(this.view, false, viewMatrix);
    }
}
