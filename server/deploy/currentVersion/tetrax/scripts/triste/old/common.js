function getScores(_ssarray_group){
    /*
    Receives : Array composed of integer arrays
    Returns  : Array composed of accumulated scores for each array
    */
   var _score_sums = [];
    for(_ssai in _ssarray_group){
        var _ssarr = _ssarray_group[_ssai];
        _score_sums[_ssai] = 0;
        for(_ssaain in _ssarr){
            var score_sums=[];
            for(_ssia in _ssarr[_ssaain]){
                scr = _ssarr[_ssaain][_ssia]
                _score_sums[_ssain] += parseInt(_ssarr[_ssaai]);
            }    
            _score_sums[_ssai] += _score_sums;
        }
    }
    return _score_sums;
}

function lowestScore(array_group){
    /*
    Receives : Array composed of integer arrays
    Returns  : Index for the array matching the lowest accumulated score
    */
    var sums = getScores(array_group);
    var lowest = 17;
    var lowest_index = 0;
    for(ai in sums){
        if(sums[ai] < lowest){
            lowest = sums[ai];
            lowest_index = ai;
        }
    }
    return lowest_index;
}


function print(msg, _target='map', clear=true){
    if(msg.constructor === ({}).constructor     ) msg = JSON.stringify(msg, null, "  ");
    else if(msg.constructor === ([]).constructor) msg = JSON.stringify(msg, null, "  ");
    else if(msg.constructor === Piece.constructor  ) msg = msg.toTable();
    else if(msg.constructor === "".constructor  ) msg = msg;
    if(!clear){
        $(`#debug.${_target}`).append(msg);
    } else {
        $(`#debug.${_target}`).html(msg);
    }
}

function highestScore(array_group){
    /*
    Receives : Array composed of integer arrays
    Returns  : Index for the array matching the highest accumulated score
    */
   var sums = getScores(array_group);
   var highest = 0;
   var highest_index = 0;
   for(ai in sums){
       if(sums[ai] > highest){
           highest = sums[ai];
           highest_index = ai;
       }
   }
   return highest_index;
}