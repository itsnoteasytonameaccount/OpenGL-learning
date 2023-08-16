import vsShader from "@WebGL/resources/shader/skybox/phone_light.vs?raw";
import fsShader from "@WebGL/resources/shader/skybox/phone_light.fs?raw";
import ProgramBase from "./ProgramBase";

export default class PhoneLightProgram extends ProgramBase {
    constructor(ctx: WebGLContext) {
        super(ctx);
    }

    getVertexShader(): GLShader {
        return ProgramBase.compileShader(vsShader, this.ctx.VERTEX_SHADER, this.ctx);
    }

    getFragmentShader(): GLShader {
        return ProgramBase.compileShader(fsShader, this.ctx.FRAGMENT_SHADER, this.ctx);
    }
}
