//
//  ScholarData.cpp
//  The_Library
//
//  Created by Adiel Fernandez on 1/3/17.
//
//

#include "ScholarData.hpp"



ScholarData::ScholarData(){
    
    
}

void ScholarData::setupData(){
    
    
    
    //set up all the scholars
    scholarList.resize(numScholars);

    scholarList[0].nameTop = "Anna M.";
    scholarList[0].nameBottom = "Cooley";
    scholarList[0].nameFull = "Anna M. Cooley";
    scholarList[0].tag = "Arts&Humanities";
    scholarList[0].dates = "(1875 - 1955)";
    scholarList[0].factSheet = "was an influential professor of Home Economics [food, nutrition, clothing, textiles, housing, applied art, household equipment, home management, family economics, child development, family relations] in the early 20th century and the author of popular books on the subject for high schools and colleges. Cooley earned diplomas at the New York Normal College (1893), Jenny Hunter Kindergarten Training School (1894) and Barnard College (1896). She received the B.S. degree and baccalaureate diploma in teaching household arts at Teachers College of Columbia University in 1903.";
    
    scholarList[1].nameTop = "Mary S.";
    scholarList[1].nameBottom = "Rose";
    scholarList[1].nameFull = "Mary S. Rose";
    scholarList[1].tag = "Health&Behavior";
    scholarList[1].dates = "(??? - ???)";
    scholarList[1].factSheet = "Professor Mary Swartz Rose was a Professor of Household Arts from 1910 to 1923 and a Professor of Nutrition from 1923 to 1940 at Teachers College. She co-created the program in Nutrition at TC with Henry Sherman, a Professor of Chemistry at Columbia University over ninety years ago. In doing so, Rose became the first full-time person to develop a program in nutrition at an American university.\n\nRose conducted extensive research on nutrition and dietetics. She designed the first nutrition laboratory devoted solely to training students in this field. In that laboratory, Rose trained others in her life's work. Some of her studies involved bringing this knowledge to the elementary schools.";
    
    scholarList[2].nameTop = "Patty Smith";
    scholarList[2].nameBottom = "Hill";
    scholarList[2].nameFull = "Patty Smith Hill";
    scholarList[2].tag = "Psychology&Cognition";
    scholarList[2].dates = "(1868 - 1946)";
    scholarList[2].factSheet = "was an American nursery school, kindergarten teacher, and key founder of the National Association Nursery Education (NANE) which now exists as the National Association For the Education of Young Children (NAEYC). Perhaps most well known as the sister of Mildred J. Hill with whom she is credited as co-writing the tune to the song Good Morning To All which became popular as Happy Birthday to You, Patty developed the Patty Hill blocks and in 1924 helped create the Institute of Child Welfare Research at Columbia University Teachers College.";

    scholarList[3].nameTop = "William Heard";
    scholarList[3].nameBottom = "Kilpatrick";
    scholarList[3].nameFull = "William Heard Kilpatrick";
    scholarList[3].tag = "STEM";
    scholarList[3].dates = "(??? - ???)";
    scholarList[3].factSheet = "was Professor Emeritus of Philosophy and Education at Teachers College where he taught from 1912 to 1937. As a highly effective proponent of progressive education, advocating project-based learning, curriculum learning, and whole child education, Kilpatrick is considered one of the most popular professors ever at Teachers College, having taught more than 35,000 students during his tenure.\n\nKilpatrick’s immensely popular article “The Project Method” (1918) made him well known among educators throughout the United States. This approach focuses on the interests of children, which advocates that by using their interests as units of study, learning becomes more relevant and meaningful. His most prominent book Foundations of Method (1925) became a widely used textbook in education courses nationwide.";
    
    scholarList[4].nameTop = "Edward";
    scholarList[4].nameBottom = "Thorndike";
    scholarList[4].nameFull = "Edward Thorndike";
    scholarList[4].tag = "Literacy&Language";
    scholarList[4].dates = "(1874 - 1949)";
    scholarList[4].factSheet = "was a psychologist who spent nearly his entire career at Teachers College, His work on Comparative psychology and the learning process led to the theory of connectionism and helped lay the scientific foundation for modern educational psychology. He also worked on solving industrial problems, such as employee exams and testing.";
    
    
    scholarList[5].nameTop = "George S.";
    scholarList[5].nameBottom = "Counts";
    scholarList[5].nameFull = "George S. Counts";
    scholarList[5].tag = "Curriculum";
    scholarList[5].dates = "(??? - ???)";
    scholarList[5].factSheet = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce non ligula quis arcu porttitor facilisis et et leo. Nullam euismod erat ipsum, in ultrices sapien bibendum sed. Vivamus ac suscipit purus. Aliquam ut viverra felis, sit amet vehicula erat. Curabitur consectetur, nunc vel convallis aliquam, dui turpis euismod est, sed fringilla metus urna id ligula. Maecenas ipsum tortor, commodo a est eget, eleifend sagittis eros. Quisque in sapien aliquet, consequat quam a, convallis metus. Vestibulum tellus ipsum, posuere ut tellus non, lacinia rutrum eros. Mauris ac laoreet urna, non ornare odio.";
    
    scholarList[6].nameTop = "John";
    scholarList[6].nameBottom = "Dewey";
    scholarList[6].nameFull = "John Dewey";
    scholarList[6].tag = "Assessment";
    scholarList[6].dates = "(??? - ???)";
    scholarList[6].factSheet = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce non ligula quis arcu porttitor facilisis et et leo. Nullam euismod erat ipsum, in ultrices sapien bibendum sed. Vivamus ac suscipit purus. Aliquam ut viverra felis, sit amet vehicula erat. Curabitur consectetur, nunc vel convallis aliquam, dui turpis euismod est, sed fringilla metus urna id ligula. Maecenas ipsum tortor, commodo a est eget, eleifend sagittis eros. Quisque in sapien aliquet, consequat quam a, convallis metus. Vestibulum tellus ipsum, posuere ut tellus non, lacinia rutrum eros. Mauris ac laoreet urna, non ornare odio.";
    
    scholarList[7].nameTop = "Jerome";
    scholarList[7].nameBottom = "Bruner";
    scholarList[7].nameFull = "Jerome Bruner";
    scholarList[7].tag = "Administration&Policy";
    scholarList[7].dates = "(??? - ???)";
    scholarList[7].factSheet = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce non ligula quis arcu porttitor facilisis et et leo. Nullam euismod erat ipsum, in ultrices sapien bibendum sed. Vivamus ac suscipit purus. Aliquam ut viverra felis, sit amet vehicula erat. Curabitur consectetur, nunc vel convallis aliquam, dui turpis euismod est, sed fringilla metus urna id ligula. Maecenas ipsum tortor, commodo a est eget, eleifend sagittis eros. Quisque in sapien aliquet, consequat quam a, convallis metus. Vestibulum tellus ipsum, posuere ut tellus non, lacinia rutrum eros. Mauris ac laoreet urna, non ornare odio.";
    
    scholarList[8].nameTop = "Benjamin";
    scholarList[8].nameBottom = "Bloom";
    scholarList[8].nameFull = "Benjamin Bloom";
    scholarList[8].tag = "Culture";
    scholarList[8].dates = "(??? - ???)";
    scholarList[8].factSheet = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce non ligula quis arcu porttitor facilisis et et leo. Nullam euismod erat ipsum, in ultrices sapien bibendum sed. Vivamus ac suscipit purus. Aliquam ut viverra felis, sit amet vehicula erat. Curabitur consectetur, nunc vel convallis aliquam, dui turpis euismod est, sed fringilla metus urna id ligula. Maecenas ipsum tortor, commodo a est eget, eleifend sagittis eros. Quisque in sapien aliquet, consequat quam a, convallis metus. Vestibulum tellus ipsum, posuere ut tellus non, lacinia rutrum eros. Mauris ac laoreet urna, non ornare odio.";
    
    scholarList[9].nameTop = "Maria";
    scholarList[9].nameBottom = "Montessori";
    scholarList[9].nameFull = "Maria Montessori";
    scholarList[9].tag = "Other Stuff";
    scholarList[9].dates = "(??? - ???)";
    scholarList[9].factSheet = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce non ligula quis arcu porttitor facilisis et et leo. Nullam euismod erat ipsum, in ultrices sapien bibendum sed. Vivamus ac suscipit purus. Aliquam ut viverra felis, sit amet vehicula erat. Curabitur consectetur, nunc vel convallis aliquam, dui turpis euismod est, sed fringilla metus urna id ligula. Maecenas ipsum tortor, commodo a est eget, eleifend sagittis eros. Quisque in sapien aliquet, consequat quam a, convallis metus. Vestibulum tellus ipsum, posuere ut tellus non, lacinia rutrum eros. Mauris ac laoreet urna, non ornare odio.";
    
    //    tagList[0] = "Arts&Humanities";
    //    tagList[1] = "Health&Behavior";
    //    tagList[2] = "Psychology&Cognition";
    //    tagList[3] = "STEM";
    //    tagList[4] = "Literacy&Language";
    //    tagList[5] = "Curriculum";
    //    tagList[6] = "Assessment";
    //    tagList[7] = "Administration&Policy";
    //    tagList[8] = "Culture";
    //    tagList[9] = "Other Stuff";

    
    
    
    //Go through the scholars and set other data
    for(int i = 0; i < scholarList.size(); i++){
        
        scholarList[i].id = i;
        
        int hueDiff = 255 / scholarList.size();
        
        ofColor tagCol;
        tagCol.setHsb( i * hueDiff, 165, 255);
        
        scholarList[i].tagCol = tagCol;
        
        //store their tags in a smaller vector for convenience
        tagList.push_back(scholarList[i].tag);
        tagColorList.push_back(tagCol);
        
    }
    
    
}


