const { describe, it } = require("mocha");
const sum = require("../index");
const assert = require('assert');
const fs = require('fs');
const fsPro = require('fs').promises;

const chai = require('chai');
const axios = require("axios");
const chaiAsset = chai.assert;
const chaiExpect = chai.expect; 
chai.should();

const supertest = require('supertest');
const app = require("../app");


describe('组测试 assert', ()=>{
  
  describe('组测试1-1', ()=>{
    it('sum() = 0', ()=>{
      assert.strictEqual(sum(), 0);
    })
    it('sum(1) = 1', ()=>{
      assert.strictEqual(sum(1), 1);
    })
    it('sum(2, 3) = 5', ()=>{
      assert.strictEqual(sum(2, 3), 5);
    })
    it('sum(4, 5, 6) = 15', ()=>{
      assert.strictEqual(sum(4, 5, 6), 15);
    })
  })
  describe('组测试1-2', ()=>{

  })
})


describe('组测试 chai', ()=>{
  it('chai asset', ()=>{ 
    const value = 'Hello'
    chaiAsset.typeOf(value, 'string')
    chaiAsset.equal(value, 'Hello')
    chaiAsset.lengthOf(value, 5);
  })

  it('chai should', ()=>{ 
    const value = 'Hello'
    value.should.exist.and.equal('Hello').and.have.length(5).and.be.a('string')
  })

  it('chai expect', ()=>{
    const value = sum(4, 5, 6)
    chaiExpect(value).to.be.most(15);
    chaiExpect(value).to.be.least(15);
    chaiExpect(value).to.be.within(14, 16);
    chaiExpect(value).to.equal(15);
  })
})

describe('组测试 异步测试', ()=>{
  it('异步操作', (done)=>{ 
    fs.readFile('./1.txt', 'utf8', (err, data)=>{
      if(err) {
        done(err)
      } else {
        assert.strictEqual(data, 'Hello')
        done()
      }
    }); 
  })

  it('异步操作 2', async ()=>{ 
    const data = await fsPro.readFile('./1.txt', 'utf8')
    assert.strictEqual(data, 'Hello')
  })
})


describe('组测试 http 测试', ()=>{
  let server
  it('异步操作', async ()=>{ 
    supertest(server).get('/').
    expect('Content-type', 'text\/html').
    expect(200, `<h1>Hello </h1>`)
  })

  /// 钩子函数
  before(()=>{
    /// 每个 describe 测试执行之前执行
    server = app.listen(3000);
  })

  after(()=>{
    server.close()
  })

  beforeEach(()=>{
    /// 每个 it 测试执行之前执行
  })
  afterEach(()=>{
    /// 每个 it 测试执行之后执行  
  })
})

/**   npm install chai
 * 
 * 
 * describe 一组测试
 * it 一个测试
*/
