import * as glMatrix from "gl-matrix"
// let glMatrix = require("gl-matrix");
class Drawer {
    public gl: WebGL2RenderingContext
    public programInfo: any
    public squareRotation: number;
    public texture: WebGLTexture | null
    constructor(ctx: WebGL2RenderingContext) {
        this.gl = ctx;
        this.squareRotation = 0;
        this.texture = this.useTexture("cubetexture.png");
    }
    initProgram(fsSource: string, vsSource: string): WebGLProgram | null {
        const gl: WebGL2RenderingContext = this.gl;
        const fragment_shader: WebGLShader | null = this.compileShader(fsSource, gl.FRAGMENT_SHADER);
        const vertex_shader: WebGLShader | null = this.compileShader(vsSource, gl.VERTEX_SHADER);
        const program: WebGLProgram | null = gl.createProgram();
        if (!program || !fragment_shader || !vertex_shader) {
            console.error("unkown error occured");
            return null;
        }
        gl.attachShader(program, fragment_shader);
        gl.attachShader(program, vertex_shader);
        gl.linkProgram(program);
        gl.deleteShader(fragment_shader);
        gl.deleteShader(vertex_shader);
        if (gl.getProgramParameter(program, gl.LINK_STATUS))
            return program;
        else {
            console.log("error occured while link");
            return null;
        }

    }
    compileShader(code: string, type: number): WebGLShader | null {
        const gl: WebGL2RenderingContext = this.gl;
        const shader: WebGLShader | null = gl.createShader(type);
        if (!shader) {
            console.error("create shader error");
            return null;
        }
        gl.shaderSource(shader, code);
        gl.compileShader(shader);
        if (gl.getShaderParameter(shader, gl.COMPILE_STATUS))
            return shader;
        else {
            console.error("compile shader error: ", gl.getShaderInfoLog(shader));
            return null;
        }
    }
    initBuffer(data: ArrayBuffer, type: number): WebGLBuffer | null {
        const gl: WebGL2RenderingContext = this.gl;
        const buffer: WebGLBuffer | null = gl.createBuffer();
        if (!buffer) {
            console.error("create buffer error");
            return null;
        }
        gl.bindBuffer(type, buffer);
        gl.bufferData(type, data, gl.STATIC_DRAW);
        return buffer;
    }
    example(deltaTime: number = 0): void {
        this.reset();
        const squareRotation = this.squareRotation;
        // ????????????
        // ????????????
        // ????????????
        // ????????????
        let vsSource: string = `
        attribute vec3 vertexPosition;
        // attribute vec4 vertexColor;
        attribute vec2 textureCoord;
        uniform mat4 projectionPosition;
        uniform mat4 modelViewPosition;
        varying lowp vec4 vColor;
        varying highp vec2 vTextureCoord;
        void main(){
            gl_Position = projectionPosition * modelViewPosition * vec4(vertexPosition, 1.0);
            // vColor = vertexColor;
            vTextureCoord = textureCoord;
        }
        `;
        let fsSource: string = `
        varying lowp vec4 vColor;
        varying highp vec2 vTextureCoord;
        uniform sampler2D uSampler;
        void main(){
            gl_FragColor = texture2D(uSampler, vec2(vTextureCoord.s, vTextureCoord.t));
            // gl_FragColor = vColor;
        }
        `;
        const program: WebGLProgram | null = this.initProgram(fsSource, vsSource);
        const gl: WebGL2RenderingContext = this.gl;
        const texture: WebGLTexture | null = this.texture;
        if (!program) {
            return;
        }
        // ???????????????
        const programInfo = {
            attribute: {
                vertexPosition: gl.getAttribLocation(program, "vertexPosition"),
                vertexColor: gl.getAttribLocation(program, "vertexColor"),
                textureCoord: gl.getAttribLocation(program, "textureCoord")
            },
            uniform: {
                projectionPosition: gl.getUniformLocation(program, "projectionPosition"),
                modelViewPosition: gl.getUniformLocation(program, "modelViewPosition"),
                uSampler: gl.getUniformLocation(program, "uSampler")
            },
            // ?????????????????????????????????xyz??????
            pointBuffer: this.initBuffer(new Float32Array([
                -1, 1, 1,
                1, 1, 1,
                1, -1, 1,
                -1, -1, 1,
                -1, 1, -1,
                1, 1, -1,
                1, -1, -1,
                -1, -1, -1,
            ]), gl.ARRAY_BUFFER),
            colorsBuffer: this.initBuffer(new Float32Array(this.createColor()), gl.ARRAY_BUFFER),
            // ???????????????
            triangleBuffer: this.initBuffer(new Uint16Array([
                0, 1, 2, 0, 2, 3,
                0, 1, 4, 1, 4, 5,
                0, 3, 4, 3, 4, 7,
                2, 6, 1, 6, 1, 5,
                2, 6, 3, 6, 3, 7,
                4, 5, 6, 4, 6, 7
            ]), gl.ELEMENT_ARRAY_BUFFER),
            textureBuffer: this.initBuffer(new Float32Array(
                [
                    // X,Y????????????????????????????????????????????????????????????4??????
                    0.0, 0.0,
                    1.0, 0.0,
                    1.0, 1.0,
                    0.0, 1.0,
                    0.0, 1.0,
                    1.0, 1.0,
                    1.0, 1.0,
                    1.0, 0.0,
                ]), gl.ARRAY_BUFFER)
        }

        /**
         * ???????????????????????????????????????
         * ??????????????????
         * 1.?????????
         * 2.???????????????????????????????????????
         * 3.???????????????
         * 4.?????????(??????????????????gl.FLOAT?????????),
         * 5.???????????????????????????????????????????????????????????????0????????????????????????????????????
         * 6.???????????????????????????
         */
        gl.bindBuffer(gl.ARRAY_BUFFER, programInfo.textureBuffer);
        gl.vertexAttribPointer(programInfo.attribute.textureCoord, 2, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(programInfo.attribute.textureCoord);

        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, programInfo.triangleBuffer);
        // gl.bindBuffer(gl.ARRAY_BUFFER, programInfo.colorsBuffer);
        // gl.vertexAttribPointer(programInfo.attribute.vertexColor, 4, gl.FLOAT, false, 0, 0);
        // gl.enableVertexAttribArray(programInfo.attribute.vertexColor);

        gl.bindBuffer(gl.ARRAY_BUFFER, programInfo.pointBuffer);
        gl.vertexAttribPointer(programInfo.attribute.vertexPosition, 3, gl.FLOAT, false, 0, 0);
        gl.enableVertexAttribArray(programInfo.attribute.vertexPosition);

        // ????????????
        gl.useProgram(program);

        gl.activeTexture(gl.TEXTURE0);
        gl.bindTexture(gl.TEXTURE_2D, texture);
        // ??????
        gl.uniform1i(programInfo.uniform.uSampler, 0);
        // ????????????
        const projectionMatrix: glMatrix.mat4 = glMatrix.mat4.create();
        glMatrix.mat4.perspective(
            projectionMatrix, // ??????
            Math.PI / 4, // ??????
            gl.canvas.width / gl.canvas.height, //?????????
            0.1, // ??????
            100 // ??????
        )
        // ????????????????????????
        gl.uniformMatrix4fv(programInfo.uniform.projectionPosition, false, projectionMatrix);
        // ??????????????????6??????
        const modelViewMatrix: glMatrix.mat4 = glMatrix.mat4.create();
        glMatrix.mat4.translate(modelViewMatrix, modelViewMatrix, [-0, 0, -6]);
        glMatrix.mat4.rotate(modelViewMatrix, modelViewMatrix, squareRotation, [0, 1, 1]);
        // ????????????????????????
        gl.uniformMatrix4fv(programInfo.uniform.modelViewPosition, false, modelViewMatrix);

        // ???????????????????????????0???????????????????????????4??????
        // gl.drawArrays(gl.TRIANGLE_STRIP, 0, 4);
        // ???????????????????????????36????????????????????????????????????
        gl.drawElements(gl.TRIANGLES, 36, gl.UNSIGNED_SHORT, 0);
        this.squareRotation += deltaTime;
    }
    createColor(): Array<number> {
        var colors: Array<number> = [
            1.0, 0.0, 0, 1.0,    // Front face: white
            1.0, 0.0, 0, 1.0,    // Front face: white
            1.0, 0.0, 0, 1.0,    // Front face: white
            1.0, 0.0, 0, 1.0,    // Front face: white
            1.0, 0.0, 0, 1.0,    // Front face: white
            1.0, 0.0, 0, 1.0,    // Front face: white
            1.0, 0.0, 0, 1.0,    // Front face: white
            1.0, 0.0, 0, 1.0,    // Front face: white
            1.0, 0.0, 0, 1.0,    // Front face: white
            0.0, 0.0, 1.0, 1.0,    // Back face: red
            0.0, 1.0, 0.0, 1.0,    // Top face: green
            0.0, 1.0, 1.0, 1.0,    // Bottom face: blue
            1.0, 1.0, 0.0, 1.0,    // Right face: yellow
            1.0, 0.0, 1.0, 1.0,    // Left face: purple
            1.0, 1.0, 1.0, 1.0,    // Left face: purple
            0.0, 0.0, 0.0, 1.0     // Left face: purple
        ];
        return colors;
    }
    reset(): void {
        const gl: WebGL2RenderingContext = this.gl;
        // ??????????????????????????????
        gl.clearColor(0, 0, 0, 1);
        // ?????????????????????1
        gl.clearDepth(1);
        // ??????????????????
        gl.enable(gl.DEPTH_TEST);           // Enable depth testing
        gl.depthFunc(gl.LEQUAL);            // Near things obscure far things
        // ??????????????????????????????????????????
        gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);
    }
    useTexture(path: string): WebGLTexture | null {
        let image: HTMLImageElement = new Image();
        const gl: WebGLRenderingContext = this.gl;
        const texture: WebGLTexture | null = gl.createTexture();
        if (!texture) {
            return texture;
        }
        gl.bindTexture(gl.TEXTURE_2D, texture);
        const level = 0;
        const internalFormat = gl.RGBA;
        const width = 1;
        const height = 1;
        const border = 0;
        const srcFormat = gl.RGBA;
        const srcType = gl.UNSIGNED_BYTE;
        const pixel = new Uint8Array([0, 0, 255, 255]);  // opaque blue
        gl.texImage2D(gl.TEXTURE_2D, level, internalFormat,
            width, height, border, srcFormat, srcType,
            pixel);
        gl.bindTexture(gl.TEXTURE_2D, null);
        image.onload = () => {
            let width = 2;
            let height = 2;
            while (width != image.width) {
                width *= 2;
                if (width > image.width) {
                    width /= 2;
                    break;
                }
            }
            while (height != image.height) {
                height *= 2;
                if (height > image.height) {
                    height /= 2;
                    break;
                }
            }
            if (width != image.width || height != image.height) {
                this.imageClip(image, width, height, () => {
                    this.initTexture(image, texture);
                });
            } else {
                this.initTexture(image, texture);
            }
        }
        image.src = path;
        return texture;
    }
    imageClip(image: HTMLImageElement, width: number, height: number, callback: EventListener) {
        console.log("clip");
        const canvas: HTMLCanvasElement = document.createElement("canvas");
        Object.assign(canvas, {
            width,
            height
        });
        const ctx: CanvasRenderingContext2D | null = canvas.getContext("2d");

        if (!ctx) {
            return;
        }
        ctx.drawImage(image, 0, 0);
        image.onload = callback;
        image.src = canvas.toDataURL("image/png");
    }
    initTexture(image: HTMLImageElement, texture: WebGLTexture): void {
        // console.log("init", image.src);
        const gl: WebGL2RenderingContext = this.gl;
        // ??????
        gl.bindTexture(gl.TEXTURE_2D, texture);
        // ????????????
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);
        // ????????????
        // ?????????????????????
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        // ?????????????????????????????????
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_NEAREST);
        // ????????????
        gl.generateMipmap(gl.TEXTURE_2D);

        // ??????
        gl.bindTexture(gl.TEXTURE_2D, null);
        this.example(0);
    }
    initProgramfromScript(fsid: string, vsid: string): WebGLProgram | null {
        const fsSource: string | undefined = document.querySelector("script#" + fsid)?.innerHTML;
        const vsSource: string | undefined = document.querySelector("script#" + vsid)?.innerHTML;
        const gl: WebGL2RenderingContext = this.gl;
        const program: WebGLProgram | null = gl.createProgram();
        let fsShader: WebGLShader | null, vsShader: WebGLShader | null;
        if (!program) {
            console.error("create program failed");
            return program;
        }
        if (fsSource && vsSource) {
            fsShader = this.compileShader(fsSource, gl.FRAGMENT_SHADER);
            vsShader = this.compileShader(vsSource, gl.VERTEX_SHADER);
            if (!fsShader || !vsShader) {
                return null;
            }
            gl.attachShader(program, vsShader);
            gl.attachShader(program, fsShader);
            gl.linkProgram(program);
            if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
                console.error("link error: ", gl.getProgramInfoLog(program));
                return null;
            }
        }
        console.error("create shader failed!");
        return null
    }
    initProgramInfo({ buffer, attribute, uniform }: any, program: WebGLProgram): any {
        const gl: WebGL2RenderingContext = this.gl;
        let result: any = {
            attribute: {

            },
            uniform: {

            },
            buffer: {

            }
        }
        let temp: WebGLBuffer | null;
        for (let [type, name, data] of buffer) {
            temp = gl.createBuffer();
            gl.bindBuffer(type, temp);
            gl.bufferData(type, data, gl.STATIC_DRAW);
            result.buffer[name] = temp;
        }
        for (let key of attribute) {
            result.attribute[key] = gl.getAttribLocation(program, key);
        }
        for (let key of uniform) {
            result.uniform[key] = gl.getUniformLocation(program, key);
        }
        return result;
    }
}