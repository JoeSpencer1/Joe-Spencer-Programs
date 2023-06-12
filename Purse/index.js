console.log('What does this do?');
let name = 'Joe';
console.log(name);
let firstName = 'Joseph', lastName = 'Spencer';
console.log(firstName,lastName);
const interestRate = 0.3;
console.log(interestRate);
let person = {
    personName: 'Andy',
    personAge: 16
};
console.log(person.personAge);
person['personAge'] = 17;
console.log(person);
let selectedColors = ['red','blue'];
console.log(selectedColors);
selectedColors[2] = 14;
console.log(selectedColors[3]);
console.log(selectedColors.length);
selectedColors[3] = 2;
console.log(selectedColors);
selectedColors.splice(3);
console.log(selectedColors);
function greet(personname){
    console.log('Hey ;)', personname);
}
greet('My Buddy');
greet(person['personName']);
function square(number){
    return number*number;
}
console.log(square(6));
