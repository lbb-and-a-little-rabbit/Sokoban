import torch

# chars mapping: attempt to be robust across different Sokoban map conventions
WALL_CHARS = set(['#'])
TARGET_CHARS = set(['.', 'T', 't'])
BOX_CHARS = set(['$', '*', 'B', 'b'])
PLAYER_CHARS = set(['@', '+', 'P', 'p'])
FLOOR_CHARS = set([' ', '_'])

def mapstr_to_tensor(mapstr, device=None):
    """
    mapstr: single-line, rows separated by '|' (as emitted by EnvServer)
    returns: 1D FloatTensor of shape (C*H*W,)
    channels: [wall, target, box, player]
    """
    rows = mapstr.split('|')
    H = len(rows)
    W = max(len(r) for r in rows)
    # pad rows to W
    grid = [list(r.ljust(W)) for r in rows]

    # channels
    wall = [[0.0]*W for _ in range(H)]
    target = [[0.0]*W for _ in range(H)]
    box = [[0.0]*W for _ in range(H)]
    player = [[0.0]*W for _ in range(H)]

    for i in range(H):
        for j in range(W):
            c = grid[i][j]
            if c in WALL_CHARS:
                wall[i][j] = 1.0
            if c in TARGET_CHARS:
                target[i][j] = 1.0
            if c in BOX_CHARS:
                box[i][j] = 1.0
            if c in PLAYER_CHARS:
                player[i][j] = 1.0
            # else floor remains zero

    # flatten channels in a consistent order
    vec = []
    for ch in (wall, target, box, player):
        for i in range(H):
            for j in range(W):
                vec.append(ch[i][j])

    tensor = torch.tensor(vec, dtype=torch.float32)
    if device is not None:
        tensor = tensor.to(device)
    return tensor

def infer_input_dim_from_map(mapstr):
    rows = mapstr.split('|')
    H = len(rows)
    W = max(len(r) for r in rows)
    C = 4
    return C * H * W
