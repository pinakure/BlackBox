def get_scores(_ssarray_group):
    """
        Receives : Array composed of integer arrays
        Returns  : Array composed of accumulated scores for each array
    """
    _score_sums = []
    for _ssai in _ssarray_group:
        _ssarr = _ssarray_group[_ssai]
        _score_sums[_ssai] = 0
        for _ssaain in _ssarr:
            score_sums=[]
            for _ssia in _ssarr[_ssaain]:
                scr = _ssarr[_ssaain][_ssia]
                _score_sums[_ssain] += int(_ssarr[_ssaai])
            _score_sums[_ssai] += _score_sums
    return _score_sums

def lowest_score(array_group):
    """
        Receives : Array composed of integer arrays
        Returns  : Index for the array matching the lowest accumulated score
    """
    sums = get_scores(array_group)
    lowest = 17
    lowest_index = 0
    for ai in sums:
        if sums[ai] < lowest:
            lowest = sums[ai]
            lowest_index = ai
    return lowest_index

def highest_score(array_group):
    """
    Receives : Array composed of integer arrays
    Returns  : Index for the array matching the highest accumulated score
    """
    sums = get_scores(array_group)
    highest = 0
    highest_index = 0
    for ai in sums:
        if sums[ai] > highest:
            highest = sums[ai]
            highest_index = ai
    return highest_index

def print(msg, _target='map', clear=True):
    import console
    if clear: console.cls()
    console.echo(msg)
