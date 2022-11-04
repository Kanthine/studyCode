import React, {Component} from 'react'
import ShallowRender from 'react-test-renderer/shallow'
import ReactTestUtil from 'react-dom/test-utils'
import App from '../App' 

describe("react-test-renderer", function(){
  it("测试用例1", function(){
    const render = new ShallowRender()
    render.render(<App/>)
    console.log('case1: ', render.getRenderOutput().props.children)

    /// 断言
    expect(render.getRenderOutput().props.children[0].type).toBe('h1')
    // expect(render.getRenderOutput().props.children[0].type).toBe('h2')
  });

  it("删除功能测试", function(){
    const app = ReactTestUtil.renderIntoDocument(<App/>)
    let todoItems = ReactTestUtil.scryRenderedDOMComponentsWithTag(app, 'li')
    console.log('case2: ', todoItems)
    let delBtn = todoItems[0].querySelector('button')
    ReactTestUtil.Simulate.click(delBtn) 

    let todoItems2 = ReactTestUtil.scryRenderedDOMComponentsWithTag(app, 'li')
    console.log('case2-2: ', todoItems2)
    expect(todoItems.length - 1).toBe(todoItems2.length)
  });

  it("添加功能测试", function(){
    const app = ReactTestUtil.renderIntoDocument(<App/>)
    let todoItems = ReactTestUtil.scryRenderedDOMComponentsWithTag(app, 'li')
    console.log('case3: ', todoItems)

    let addInput = ReactTestUtil.scryRenderedDOMComponentsWithTag(app, 'input')
    addInput.values = todoItems.length
    
    let addBtn = ReactTestUtil.findRenderedDOMComponentWithClass(app, 'add')
    ReactTestUtil.Simulate.click(addBtn) 

    let todoItems2 = ReactTestUtil.scryRenderedDOMComponentsWithTag(app, 'li')
    console.log('case3-2: ', todoItems2)

    expect(todoItems.length + 1).toBe(todoItems2.length)
  });

})


/**
 * npm install react-test-renderer
 * 
 * 单元测试：每次版本迭代、快速的进行回归测试
 * 
 * 
*/