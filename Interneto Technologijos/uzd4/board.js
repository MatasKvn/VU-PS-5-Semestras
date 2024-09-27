'use stric'

import "./lib/jquery.js"

import { SudokuApi } from "./sudokuApi.js"

export let boardData = {
    width: undefined,
    height: undefined
}
export let boardState = undefined

function onCorrectSolve() {
    console.log(':)')
    $('#congrats-modal').show()
}

let timer
function onIncorrectSolve() {
    const indicatorElement = $('#indicator-text')
    indicatorElement.show()
    indicatorElement.text('Solution is not correct!')
    if (timer) clearTimeout(timer)
    timer = setTimeout(() => {
            indicatorElement.hide()
        },
        2000
    )
}

export function resetBoard() {
    Array.from(boardState).forEach((character, index) => {
        const value = boardState.at(index)
        $(`#cell-${index}`).val(value !== 'x' ? value : '')
    })
}

export function cheat() {
    SudokuApi.fetchBoardSolution()
        .then((solution) => {
            Array.from(boardState).forEach((character, index) => {
                const value = solution.at(index)
                $(`#cell-${index}`).val(value !== 'x' ? value : '')
            })
        })
}

export function submitSolution() {
    SudokuApi.fetchBoardSolution()
        .then((solution) => {
            const isSolved = Array.from(boardState)
                .map((character, index) => 
                    $(`#cell-${index}`).val() === solution.at(index)
                )
                .every((bool) => bool)
            isSolved ? onCorrectSolve() : onIncorrectSolve()
        })
}

export function loadBoard() {
    SudokuApi.fetchBoardData()
    .then((res) => {
        const { width, height, board } = res
        boardData = {
            width,
            height
        }
        populateBoard({
            width, 
            height,
            boardData: board
        })
        $('#start-btn').text('Play')
    })
}

export function populateBoard({ width, height, boardData }) {
    boardState = boardData // Correct Answer
    
    const sudokuContainer = document.getElementById('sudoku-container')
    $('#sudoku-container').empty()
    Array.from({ length: height }, (val, rowIndex) => {
        Array.from({ length: width }, (val, colIndex) => {
            const inputElement = document.createElement('input')
            const value = boardState.at((rowIndex * width) + colIndex)
            inputElement.value = value !== 'x' ? value : ''
            inputElement.className = 'sudoku-cell'
            inputElement.addEventListener('keydown', (e) => {
                    if (e.key < '0' || e.key > '9' && e.key != 'Backspace'){
                        e.preventDefault();
                    }
                })
            const cellNr = rowIndex * width + colIndex
            inputElement.id = `cell-${cellNr}`
            sudokuContainer.insertAdjacentElement('beforeend', inputElement)
            sudokuContainer.style.minWidth = `${2 * width}rem`
        })
        const lineBreak = document.createElement('br')
        sudokuContainer.insertAdjacentElement('beforeend', lineBreak)
    })
}
