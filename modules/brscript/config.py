def can_build(env, platform):
    env.module_add_dependencies("brscript", ["jsonrpc", "websocket"], True)
    return True


def configure(env):
    pass


def get_doc_classes():
    return [
        "@BRScript",
        "BRScript",
        "BRScriptSyntaxHighlighter",
    ]


def get_doc_path():
    return "doc_classes"
