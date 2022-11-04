function reducer(preState={
  list: [],
  chain: [],
  account:{}
}, action={}) {
  var newState = {... preState};
  switch(action.type) {
    case 'change-list':
      console.log('newList ', action.payload);
      newState.list = action.payload;
      return newState;
    case 'change-Account':
      console.log('Account ', action.payload);
      newState.account = action.payload;
      return newState;
    case 'change-Chain':
      console.log('Chain ', action.payload);
      newState.chain = action.payload;
      return newState;
    default:
      return preState;
  }
}

export default reducer;