import ProgramBase from "@WebGL/program/ProgramBase";
import Objects from "./Objects";
import Texture from "@WebGL/tools/Texture";
import { ATTRIBUTE_LOCATION_UNDEFINED } from "@WebGL/definition/constant";

const arrIndices = [
    0, 1, 2, 0, 2, 3, 0, 1, 4, 1, 4, 5, 0, 3, 4, 3, 4, 7, 2, 6, 1, 6, 1, 5, 2, 6, 3, 6, 3, 7, 4, 5, 6, 4, 6, 7,
];

const arrVertex = [-1, 1, 1, 1, 1, 1, 1, -1, 1, -1, -1, 1, -1, 1, -1, 1, 1, -1, 1, -1, -1, -1, -1, -1];

const arrTexCroods = [
    // 这个数据有点问题
    0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0,
];

const boxDatas = [
    -0.5, -0.5, -0.5, 0.0, 0.0, -1.0, 0.0, 0.0, 0.5, 0.5, -0.5, 0.0, 0.0, -1.0, 1.0, 1.0, 0.5, -0.5, -0.5, 0.0, 0.0,
    -1.0, 1.0, 0.0, 0.5, 0.5, -0.5, 0.0, 0.0, -1.0, 1.0, 1.0, -0.5, -0.5, -0.5, 0.0, 0.0, -1.0, 0.0, 0.0, -0.5, 0.5,
    -0.5, 0.0, 0.0, -1.0, 0.0, 1.0,

    -0.5, -0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.5, -0.5, 0.5, 0.0, 0.0, 1.0, 1.0, 0.0, 0.5, 0.5, 0.5, 0.0, 0.0, 1.0,
    1.0, 1.0, 0.5, 0.5, 0.5, 0.0, 0.0, 1.0, 1.0, 1.0, -0.5, 0.5, 0.5, 0.0, 0.0, 1.0, 0.0, 1.0, -0.5, -0.5, 0.5, 0.0,
    0.0, 1.0, 0.0, 0.0,

    -0.5, 0.5, 0.5, -1.0, 0.0, 0.0, 1.0, 0.0, -0.5, 0.5, -0.5, -1.0, 0.0, 0.0, 1.0, 1.0, -0.5, -0.5, -0.5, -1.0, 0.0,
    0.0, 0.0, 1.0, -0.5, -0.5, -0.5, -1.0, 0.0, 0.0, 0.0, 1.0, -0.5, -0.5, 0.5, -1.0, 0.0, 0.0, 0.0, 0.0, -0.5, 0.5,
    0.5, -1.0, 0.0, 0.0, 1.0, 0.0,

    0.5, 0.5, 0.5, 1.0, 0.0, 0.0, 1.0, 0.0, 0.5, -0.5, -0.5, 1.0, 0.0, 0.0, 0.0, 1.0, 0.5, 0.5, -0.5, 1.0, 0.0, 0.0,
    1.0, 1.0, 0.5, -0.5, -0.5, 1.0, 0.0, 0.0, 0.0, 1.0, 0.5, 0.5, 0.5, 1.0, 0.0, 0.0, 1.0, 0.0, 0.5, -0.5, 0.5, 1.0,
    0.0, 0.0, 0.0, 0.0,

    -0.5, -0.5, -0.5, 0.0, -1.0, 0.0, 0.0, 1.0, 0.5, -0.5, -0.5, 0.0, -1.0, 0.0, 1.0, 1.0, 0.5, -0.5, 0.5, 0.0, -1.0,
    0.0, 1.0, 0.0, 0.5, -0.5, 0.5, 0.0, -1.0, 0.0, 1.0, 0.0, -0.5, -0.5, 0.5, 0.0, -1.0, 0.0, 0.0, 0.0, -0.5, -0.5,
    -0.5, 0.0, -1.0, 0.0, 0.0, 1.0,

    -0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0, 0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 1.0, 0.0, 0.5, 0.5, -0.5, 0.0, 1.0, 0.0,
    1.0, 1.0, 0.5, 0.5, 0.5, 0.0, 1.0, 0.0, 1.0, 0.0, -0.5, 0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0, -0.5, 0.5, 0.5, 0.0,
    1.0, 0.0, 0.0, 0.0,
];

export default class Box extends Objects {
    vertex: WebGLBuffer;
    texCroods: WebGLBuffer;
    indices: WebGLBuffer;
    textureImage: HTMLImageElement = null;
    texture: WebGLTexture = null;

    constructor(ctx: WebGLContext, textureImage: string | null, drawElements: boolean = true) {
        super(ctx);
        if (drawElements) {
            this.createDataBuffer();
        } else {
            this.createDrawArraysDataBuffer();
        }
        if (textureImage !== null) {
            Texture.loadImage(textureImage).then((image) => (this.textureImage = image));
        }
    }

    createDataBuffer(): void {
        this.vertex = this.getVertex();
        this.indices = this.getIndices();
        this.texCroods = this.getTextureCroods();
    }

    createDrawArraysDataBuffer(): void {
        const gl = this.ctx;
        const datas = new Float32Array(boxDatas);
        this.vertex = Objects.initBufferData(datas, gl.ARRAY_BUFFER, gl);
    }

    /**
     * 顶点纹理坐标
     *
     * @returns {WebGLBuffer} 顶点纹理坐标
     */
    private getTextureCroods(): WebGLBuffer {
        const gl = this.ctx;
        const texCroods = new Float32Array(arrTexCroods);
        return Objects.initBufferData(texCroods, gl.ARRAY_BUFFER, gl);
    }

    /**
     * 获取顶点数据
     * @returns {WebGLBuffer} 顶点数据数组
     */
    private getVertex(): WebGLBuffer {
        const gl = this.ctx;
        const vertex = new Float32Array(arrVertex);
        return Objects.initBufferData(vertex, gl.ARRAY_BUFFER, gl);
    }

    /**
     * 获取顶点索引数据
     * @returns {WebGLBuffer} 顶点索引数据数组
     */
    private getIndices(): WebGLBuffer {
        const gl = this.ctx;
        const indices = new Uint16Array(arrIndices);
        return Objects.initBufferData(indices, gl.ELEMENT_ARRAY_BUFFER, gl);
    }

    draw(program: ProgramBase, textureId: WebGLTextureId = this.ctx.TEXTURE0) {
        const gl = this.ctx;
        if (this.textureImage && this.texture === null) {
            this.texture = Texture.load2DTexture(textureId, this.textureImage, gl);
        }

        const vertex = program.getAttributeLocation("vertex");
        const texCoords = program.getAttributeLocation("texCroods");
        const normals = program.getAttributeLocation("normals");
        program.enableVertexArray(this.vertex, vertex, 3, 8, 0);
        if (normals !== ATTRIBUTE_LOCATION_UNDEFINED) {
            program.enableVertexArray(this.vertex, normals, 3, 8, 3);
        }
        program.enableVertexArray(this.vertex, texCoords, 2, 8, 6);
        // gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, this.indices);
        gl.drawArrays(gl.TRIANGLES, 0, 36);
    }

    getIndicesSize(): number {
        return arrIndices.length;
    }
}
