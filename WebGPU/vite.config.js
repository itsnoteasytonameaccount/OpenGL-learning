import path from "path";

export default {
    resolve: {
        alias: [
            {
                find: "@WebGPU",
                replacement: path.resolve(__dirname, "."),
            },
            {
                find: "@texture",
                replacement: path.resolve(__dirname, "resources/texture"),
            },
            {
                find: "@GPUShader",
                replacement: path.resolve(__dirname, "resources/shader"),
            },
        ],
    },
};
