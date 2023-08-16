import ProgramBase from "@WebGL/program/ProgramBase";
import Box from "./Box";
import Texture from "@WebGL/tools/Texture";

export default class SkyBox extends Box {
    constructor(ctx: WebGLContext, textureImage: string) {
        super(ctx, textureImage, false);
        this.scale(2, 2, 2);
    }

    draw(program: ProgramBase, textureId: WebGLTextureId = this.ctx.TEXTURE0): void {
        const gl: WebGL2RenderingContext = <WebGL2RenderingContext>this.ctx;
        if (this.textureImage && !this.texture) {
            Texture.handleCubeMapsLoad(
                this.textureImage,
                (images: HTMLImageElement[]) =>
                    (this.texture = Texture.loadCubeMapsTexture(textureId, images, <WebGL2RenderingContext>gl))
            );
        }

        const vertex = program.getAttributeLocation("aPos");
        program.enableVertexArray(this.vertex, vertex, 3, 8, 0);
        gl.drawArrays(gl.TRIANGLES, 0, 36);
    }
}
