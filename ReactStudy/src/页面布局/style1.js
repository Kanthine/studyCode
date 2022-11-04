// 获取输入框的值 / 使用状态
import React, {Component} from 'react'
import {
  View,
  StyleSheet,
} from 'react-native';

export default class StyleDemo1 extends Component {

  render(){
    return (<View>

      <View></View>
      <View></View>
    </View>)
  }
}


const styles = StyleSheet.create({
  container:{
    backgroundColor:'red',
    width:375,
  },
  top: {
    backgroundColor:'yellow',
    width:300,
    height:250,
    margin:30,
  },
  bottom:{
    
  }
});