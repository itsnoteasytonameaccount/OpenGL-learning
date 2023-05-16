import vsShader from "@shader/basic_program/basic_program.vs?raw";
import fsShader from "@shader/basic_program/basic_program.fs?raw";
import { mat4 } from "gl-matrix";
import ProgramBase from "./ProgramBase";

export default class BasicProgram extends ProgramBase {
    projectionMatrixLocation: WebGLUniformLocation;
    viewMatrixLoaction: WebGLUniformLocation;
    sampler: WebGLUniformLocation;
    readonly vertexLocation: number = 0;
    readonly texCroodLocation: number = 1; // 若未定义layout，可以直接使用gl.getAttribLocation获取

    constructor(ctx: WebGLContext) {
        super(ctx);
        this.initLocation();
    }

    initLocation() {
        const gl = this.ctx;
        const program = this.program;
        this.projectionMatrixLocation = gl.getUniformLocation(program, "projectionMatrix");
        this.viewMatrixLoaction = gl.getUniformLocation(program, "modelViewMatrix");
        this.sampler = gl.getUniformLocation(program, "uSampler");
    }

    getVertexShader(): GLShader {
        return ProgramBase.compileShader(vsShader, this.ctx.VERTEX_SHADER, this.ctx);
    }

    getFragmentShader(): GLShader {
        return ProgramBase.compileShader(fsShader, this.ctx.FRAGMENT_SHADER, this.ctx);
    }

    fillUniformProjectionMatrix(projectionMatrix: mat4): void {
        this.ctx.uniformMatrix4fv(this.projectionMatrixLocation, false, projectionMatrix);
    }

    fillUniformViewMatrix(viewMatrix: mat4): void {
        this.ctx.uniformMatrix4fv(this.viewMatrixLoaction, false, viewMatrix);
    }

    bindVertexPointer(vertex: WebGLBuffer) {
        this.enableVertexArray(vertex, this.vertexLocation, 3);
    }

    bindTexturePointer(texCoords: WebGLBuffer) {
        this.enableVertexArray(texCoords, this.texCroodLocation, 2);
    }

    useSampler(samplerIndex: number) {
        this.ctx.uniform1i(this.sampler, samplerIndex);
    }
}
