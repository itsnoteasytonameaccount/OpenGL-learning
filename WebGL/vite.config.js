import path from "path";

export default {
    resolve: {
        alias: [
            {
                find: "@WebGL",
                replacement: path.resolve(__dirname, "."),
            },
            {
                find: "@texture",
                replacement: path.resolve(__dirname, "resources/texture"),
            },
            {
                find: "@shader",
                replacement: path.resolve(__dirname, "resources/shader"),
            },
        ],
    },
};
