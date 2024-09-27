'use strict'

import './lib/jquery.js'

import { SudokuApi } from './sudokuApi.js'
import { boardState, cheat, loadBoard, resetBoard, submitSolution } from './board.js'
export let isCheat = false

loadBoard()

document.onkeydown = (e) => {
    if (e.key === 'c') {
        isCheat = !isCheat
        if (isCheat) {
            cheat()
        } else {
            resetBoard()
        }
    }
}

$('#start-btn').click(() => {
    if (boardState) {
        $('#loading-modal').hide()
    }
})

$('#reset-btn').click(() => {
    resetBoard()
})

$('#submit-solution-btn').click(() => {
    submitSolution()
})

$('#replay-btn').click(() => {
    window.location.reload()
})

