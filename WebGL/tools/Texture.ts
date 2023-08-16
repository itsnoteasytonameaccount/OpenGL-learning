const CUBE_MAPS_POSY_INDEX = 1;
const CUBE_MAPS_NEGX_INDEX = 4;
const CUBE_MAPS_NEGZ_INDEX = 5;
const CUBE_MAPS_POSX_INDEX = 6;
const CUBE_MAPS_POSZ_INDEX = 7;
const CUBE_MAPS_NEGY_INDEX = 9;
const CUBE_MAPS_INDICES = [
    CUBE_MAPS_POSX_INDEX,
    CUBE_MAPS_NEGX_INDEX,
    CUBE_MAPS_POSY_INDEX,
    CUBE_MAPS_NEGY_INDEX,
    CUBE_MAPS_NEGZ_INDEX,
    CUBE_MAPS_POSZ_INDEX,
];

export default class Texture {
    static load2DTexture(textureId: WebGLTextureId, image: HTMLImageElement, gl: WebGLContext): WebGLTexture {
        const texture = gl.createTexture();
        gl.activeTexture(textureId);
        gl.bindTexture(gl.TEXTURE_2D, texture);
        gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.MIRRORED_REPEAT);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.MIRRORED_REPEAT);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR_MIPMAP_NEAREST);
        gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        gl.generateMipmap(gl.TEXTURE_2D);
        return texture;
    }

    static loadCubeMapsTexture(
        textureId: WebGLTextureId,
        images: HTMLImageElement[],
        gl: WebGL2RenderingContext
    ): WebGLTexture {
        const texture = gl.createTexture();
        gl.activeTexture(textureId);
        gl.bindTexture(gl.TEXTURE_CUBE_MAP, texture);
        for (let i = 0; i < images.length; i++) {
            gl.texImage2D(gl.TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, images[i]);
        }
        gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_WRAP_R, gl.CLAMP_TO_EDGE);
        gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MIN_FILTER, gl.LINEAR);
        gl.texParameteri(gl.TEXTURE_CUBE_MAP, gl.TEXTURE_MAG_FILTER, gl.LINEAR);
        return texture;
    }

    static loadImage(path: string, clip: boolean = false): Promise<HTMLImageElement> {
        const image = new Image();
        return new Promise((resolve) => {
            image.onload = () => {
                if (clip) {
                    this.handleImageLoad(image, resolve);
                } else {
                    resolve(image);
                }
            };
            image.src = path;
        });
    }

    static loadCubeMaps(path: string): Promise<Array<HTMLImageElement>> {
        const image = new Image();
        return new Promise((resolve) => {
            image.onload = () => this.handleCubeMapsLoad(image, resolve);
            image.src = path;
        });
    }

    static handleCubeMapsLoad(image: HTMLImageElement, callback: Function): void {
        const { width, height } = image;
        console.log("width, height", width, height);
        if (Math.floor(height / 3) !== Math.floor(width / 4)) {
            console.log("can't resolve image in this size");
            callback([]);
        }
        const singleTexSize = height / 3;
        const tasks = [];
        for (let i = 0; i < CUBE_MAPS_INDICES.length; i++) {
            const index = CUBE_MAPS_INDICES[i];
            const x = (index % 4) * singleTexSize;
            const y = Math.floor(index / 4) * singleTexSize;
            tasks.push(this.imageClip(image, singleTexSize, singleTexSize, -x, -y));
        }
        Promise.all(tasks).then((arr) => callback(arr));
    }

    static handleImageLoad(image: HTMLImageElement, callback: Function): void {
        let width = 2;
        let height = 2;
        while (width <= image.width) width *= 2;
        while (height <= image.height) height *= 2;

        // 纹理的宽高值需要等于2的n次幂
        this.imageClip(image, width, height).then((image) => {
            console.log("image loaded!");
            callback(image);
        });
    }

    static imageClip(
        image: HTMLImageElement,
        width: number,
        height: number,
        x: number = 0,
        y: number = 0
    ): Promise<HTMLImageElement> {
        if (image.height === height && image.width === width) {
            return Promise.resolve(image);
        }
        const canvas: HTMLCanvasElement = document.createElement("canvas");
        Object.assign(canvas, {
            width,
            height,
        });
        const ctx: CanvasRenderingContext2D | null = canvas.getContext("2d");
        if (!ctx) {
            Promise.reject(new Error("failed to create 2d ctx!"));
        }
        ctx.drawImage(image, x, y);
        console.log("image cliped", height, width, x, y);
        return new Promise((resolve) => {
            const clipedImage = new Image();
            clipedImage.onload = () => resolve(clipedImage);
            clipedImage.src = canvas.toDataURL("image/png");
        });
    }
}
