import vsShader from "@shader/basic_program/basic_program.vs?raw";
import fsShader from "@shader/basic_program/basic_program.fs?raw";
import { mat4 } from "gl-matrix";
import ProgramBase from "./ProgramBase";

export default class BasicProgram extends ProgramBase {
    projectionMatrixLocation: WebGLUniformLocation;
    modelViewMatrixLocation: WebGLUniformLocation;
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
        this.modelViewMatrixLocation = gl.getUniformLocation(program, "modelViewMatrix");
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

    fillUniformModelViewMatrix(viewMatrix: mat4): void {
        this.ctx.uniformMatrix4fv(this.modelViewMatrixLocation, false, viewMatrix);
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

    getAttributeLocation(name: string): number {
        switch (name) {
            case "vertex":
                return this.vertexLocation;
            case "texCroods":
                return this.texCroodLocation;
            default:
                return super.getAttributeLocation(name);
        }
    }

    setParameters(attributes: AttributeDescriptor[], uniforms: UnifromDescriptor[]) {
        attributes.forEach((descriptor) => {
            switch (descriptor.name) {
                case "vertex":
                    this.bindVertexPointer(descriptor.value);
                    break;
                case "texCroods":
                    this.bindTexturePointer(descriptor.value);
                    break;
            }
        });

        uniforms.forEach((descriptor) => {
            switch (descriptor.name) {
                case "modelViewMatrix":
                    this.fillUniformModelViewMatrix(<mat4>descriptor.value);
                    break;
                case "projectionMatrix":
                    this.fillUniformProjectionMatrix(<mat4>descriptor.value);
                    break;
                case "samplerId":
                    this.useSampler(<number>descriptor.value);
                    break;
            }
        });
    }
}
