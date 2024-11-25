def can_build(env, platform):
    return env.get("use_mimalloc", False)


def configure(env):
    pass
