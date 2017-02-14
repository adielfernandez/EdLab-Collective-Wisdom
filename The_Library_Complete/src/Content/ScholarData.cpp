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
    scholarList[0].dates = "(1875 - 1955)";
    scholarList[0].factSheet = "was an influential professor of Home Economics [food, nutrition, clothing, textiles, housing, applied art, household equipment, home management, family economics, child development, family relations] in the early 20th century and the author of popular books on the subject for high schools and colleges. Cooley earned diplomas at the New York Normal College (1893), Jenny Hunter Kindergarten Training School (1894) and Barnard College (1896). She received the B.S. degree and baccalaureate diploma in teaching household arts at Teachers College of Columbia University in 1903.";
    scholarList[0].works = "Food truck neutra you probably haven't heard of them schlitz. Retro viral af fanny pack normcore, twee hella jean shorts art party. Next level polaroid leggings literally. Pop-up distillery slow-carb, waistcoat disrupt chia tilde knausgaard paleo. Four loko whatever paleo pickled vice portland.\nLocavore lumbersexual neutra iPhone pour-over intelligentsia. Fashion axe XOXO vegan snackwave food truck, organic shoreditch. Brooklyn umami hashtag normcore fap. Vexillologist godard polaroid, meggings pinterest copper mug fingerstache chicharrones cronut.";
    
    scholarList[1].nameTop = "Mary S.";
    scholarList[1].nameBottom = "Rose";
    scholarList[1].nameFull = "Mary S. Rose";
    scholarList[1].dates = "(??? - ???)";
    scholarList[1].factSheet = "Professor Mary Swartz Rose was a Professor of Household Arts from 1910 to 1923 and a Professor of Nutrition from 1923 to 1940 at Teachers College. She co-created the program in Nutrition at TC with Henry Sherman, a Professor of Chemistry at Columbia University over ninety years ago. In doing so, Rose became the first full-time person to develop a program in nutrition at an American university.\n\nRose conducted extensive research on nutrition and dietetics. She designed the first nutrition laboratory devoted solely to training students in this field. In that laboratory, Rose trained others in her life's work. Some of her studies involved bringing this knowledge to the elementary schools.";
    scholarList[1].works = "Food truck neutra you probably haven't heard of them schlitz. Retro viral af fanny pack normcore, twee hella jean shorts art party. Next level polaroid leggings literally. Pop-up distillery slow-carb, waistcoat disrupt chia tilde knausgaard paleo. Four loko whatever paleo pickled vice portland.\nLocavore lumbersexual neutra iPhone pour-over intelligentsia. Fashion axe XOXO vegan snackwave food truck, organic shoreditch. Brooklyn umami hashtag normcore fap. Vexillologist godard polaroid, meggings pinterest copper mug fingerstache chicharrones cronut.";
    
    
    scholarList[2].nameTop = "Patty Smith";
    scholarList[2].nameBottom = "Hill";
    scholarList[2].nameFull = "Patty Smith Hill";
    scholarList[2].dates = "(1868 - 1946)";
    scholarList[2].factSheet = "was an American nursery school, kindergarten teacher, and key founder of the National Association Nursery Education (NANE) which now exists as the National Association For the Education of Young Children (NAEYC). Perhaps most well known as the sister of Mildred J. Hill with whom she is credited as co-writing the tune to the song Good Morning To All which became popular as Happy Birthday to You, Patty developed the Patty Hill blocks and in 1924 helped create the Institute of Child Welfare Research at Columbia University Teachers College.";
    scholarList[2].works = "Food truck neutra you probably haven't heard of them schlitz. Retro viral af fanny pack normcore, twee hella jean shorts art party. Next level polaroid leggings literally. Pop-up distillery slow-carb, waistcoat disrupt chia tilde knausgaard paleo. Four loko whatever paleo pickled vice portland.\nLocavore lumbersexual neutra iPhone pour-over intelligentsia. Fashion axe XOXO vegan snackwave food truck, organic shoreditch. Brooklyn umami hashtag normcore fap. Vexillologist godard polaroid, meggings pinterest copper mug fingerstache chicharrones cronut.";
    
    
    scholarList[3].nameTop = "William Heard";
    scholarList[3].nameBottom = "Kilpatrick";
    scholarList[3].nameFull = "William Heard Kilpatrick";
    scholarList[3].dates = "(??? - ???)";
    scholarList[3].factSheet = "was Professor Emeritus of Philosophy and Education at Teachers College where he taught from 1912 to 1937. As a highly effective proponent of progressive education, advocating project-based learning, curriculum learning, and whole child education, Kilpatrick is considered one of the most popular professors ever at Teachers College, having taught more than 35,000 students during his tenure.\n\nKilpatrick's immensely popular article The Project Method (1918) made him well known among educators throughout the United States. This approach focuses on the interests of children, which advocates that by using their interests as units of study, learning becomes more relevant and meaningful. His most prominent book Foundations of Method (1925) became a widely used textbook in education courses nationwide.";
    scholarList[3].works = "Food truck neutra you probably haven't heard of them schlitz. Retro viral af fanny pack normcore, twee hella jean shorts art party. Next level polaroid leggings literally. Pop-up distillery slow-carb, waistcoat disrupt chia tilde knausgaard paleo. Four loko whatever paleo pickled vice portland.\nLocavore lumbersexual neutra iPhone pour-over intelligentsia. Fashion axe XOXO vegan snackwave food truck, organic shoreditch. Brooklyn umami hashtag normcore fap. Vexillologist godard polaroid, meggings pinterest copper mug fingerstache chicharrones cronut.";
    
    scholarList[4].nameTop = "Edward";
    scholarList[4].nameBottom = "Thorndike";
    scholarList[4].nameFull = "Edward Thorndike";
    scholarList[4].dates = "(1874 - 1949)";
    scholarList[4].factSheet = "was a psychologist who spent nearly his entire career at Teachers College, His work on Comparative psychology and the learning process led to the theory of connectionism and helped lay the scientific foundation for modern educational psychology. He also worked on solving industrial problems, such as employee exams and testing.";
    scholarList[4].works = "Food truck neutra you probably haven't heard of them schlitz. Retro viral af fanny pack normcore, twee hella jean shorts art party. Next level polaroid leggings literally. Pop-up distillery slow-carb, waistcoat disrupt chia tilde knausgaard paleo. Four loko whatever paleo pickled vice portland.\nLocavore lumbersexual neutra iPhone pour-over intelligentsia. Fashion axe XOXO vegan snackwave food truck, organic shoreditch. Brooklyn umami hashtag normcore fap. Vexillologist godard polaroid, meggings pinterest copper mug fingerstache chicharrones cronut.";
    
    
    scholarList[5].nameTop = "George S.";
    scholarList[5].nameBottom = "Counts";
    scholarList[5].nameFull = "George S. Counts";
    scholarList[5].dates = "(??? - ???)";
    scholarList[5].factSheet = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce non ligula quis arcu porttitor facilisis et et leo. Nullam euismod erat ipsum, in ultrices sapien bibendum sed. Vivamus ac suscipit purus. Aliquam ut viverra felis, sit amet vehicula erat. Curabitur consectetur, nunc vel convallis aliquam, dui turpis euismod est, sed fringilla metus urna id ligula. Maecenas ipsum tortor, commodo a est eget, eleifend sagittis eros. Quisque in sapien aliquet, consequat quam a, convallis metus. Vestibulum tellus ipsum, posuere ut tellus non, lacinia rutrum eros. Mauris ac laoreet urna, non ornare odio.";
    scholarList[5].works = "Food truck neutra you probably haven't heard of them schlitz. Retro viral af fanny pack normcore, twee hella jean shorts art party. Next level polaroid leggings literally. Pop-up distillery slow-carb, waistcoat disrupt chia tilde knausgaard paleo. Four loko whatever paleo pickled vice portland.\nLocavore lumbersexual neutra iPhone pour-over intelligentsia. Fashion axe XOXO vegan snackwave food truck, organic shoreditch. Brooklyn umami hashtag normcore fap. Vexillologist godard polaroid, meggings pinterest copper mug fingerstache chicharrones cronut.";
    
    scholarList[6].nameTop = "John";
    scholarList[6].nameBottom = "Dewey";
    scholarList[6].nameFull = "John Dewey";
    scholarList[6].dates = "(??? - ???)";
    scholarList[6].factSheet = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce non ligula quis arcu porttitor facilisis et et leo. Nullam euismod erat ipsum, in ultrices sapien bibendum sed. Vivamus ac suscipit purus. Aliquam ut viverra felis, sit amet vehicula erat. Curabitur consectetur, nunc vel convallis aliquam, dui turpis euismod est, sed fringilla metus urna id ligula. Maecenas ipsum tortor, commodo a est eget, eleifend sagittis eros. Quisque in sapien aliquet, consequat quam a, convallis metus. Vestibulum tellus ipsum, posuere ut tellus non, lacinia rutrum eros. Mauris ac laoreet urna, non ornare odio.";
    scholarList[6].works = "Food truck neutra you probably haven't heard of them schlitz. Retro viral af fanny pack normcore, twee hella jean shorts art party. Next level polaroid leggings literally. Pop-up distillery slow-carb, waistcoat disrupt chia tilde knausgaard paleo. Four loko whatever paleo pickled vice portland.\nLocavore lumbersexual neutra iPhone pour-over intelligentsia. Fashion axe XOXO vegan snackwave food truck, organic shoreditch. Brooklyn umami hashtag normcore fap. Vexillologist godard polaroid, meggings pinterest copper mug fingerstache chicharrones cronut.";
    
    
    scholarList[7].nameTop = "Jerome";
    scholarList[7].nameBottom = "Bruner";
    scholarList[7].nameFull = "Jerome Bruner";
    scholarList[7].dates = "(??? - ???)";
    scholarList[7].factSheet = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce non ligula quis arcu porttitor facilisis et et leo. Nullam euismod erat ipsum, in ultrices sapien bibendum sed. Vivamus ac suscipit purus. Aliquam ut viverra felis, sit amet vehicula erat. Curabitur consectetur, nunc vel convallis aliquam, dui turpis euismod est, sed fringilla metus urna id ligula. Maecenas ipsum tortor, commodo a est eget, eleifend sagittis eros. Quisque in sapien aliquet, consequat quam a, convallis metus. Vestibulum tellus ipsum, posuere ut tellus non, lacinia rutrum eros. Mauris ac laoreet urna, non ornare odio.";
    scholarList[7].works = "Food truck neutra you probably haven't heard of them schlitz. Retro viral af fanny pack normcore, twee hella jean shorts art party. Next level polaroid leggings literally. Pop-up distillery slow-carb, waistcoat disrupt chia tilde knausgaard paleo. Four loko whatever paleo pickled vice portland.\nLocavore lumbersexual neutra iPhone pour-over intelligentsia. Fashion axe XOXO vegan snackwave food truck, organic shoreditch. Brooklyn umami hashtag normcore fap. Vexillologist godard polaroid, meggings pinterest copper mug fingerstache chicharrones cronut.";
    
    
    scholarList[8].nameTop = "Benjamin";
    scholarList[8].nameBottom = "Bloom";
    scholarList[8].nameFull = "Benjamin Bloom";
    scholarList[8].dates = "(??? - ???)";
    scholarList[8].factSheet = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce non ligula quis arcu porttitor facilisis et et leo. Nullam euismod erat ipsum, in ultrices sapien bibendum sed. Vivamus ac suscipit purus. Aliquam ut viverra felis, sit amet vehicula erat. Curabitur consectetur, nunc vel convallis aliquam, dui turpis euismod est, sed fringilla metus urna id ligula. Maecenas ipsum tortor, commodo a est eget, eleifend sagittis eros. Quisque in sapien aliquet, consequat quam a, convallis metus. Vestibulum tellus ipsum, posuere ut tellus non, lacinia rutrum eros. Mauris ac laoreet urna, non ornare odio.";
    scholarList[8].works = "Food truck neutra you probably haven't heard of them schlitz. Retro viral af fanny pack normcore, twee hella jean shorts art party. Next level polaroid leggings literally. Pop-up distillery slow-carb, waistcoat disrupt chia tilde knausgaard paleo. Four loko whatever paleo pickled vice portland.\nLocavore lumbersexual neutra iPhone pour-over intelligentsia. Fashion axe XOXO vegan snackwave food truck, organic shoreditch. Brooklyn umami hashtag normcore fap. Vexillologist godard polaroid, meggings pinterest copper mug fingerstache chicharrones cronut.";
    
    
    scholarList[9].nameTop = "Maria";
    scholarList[9].nameBottom = "Montessori";
    scholarList[9].nameFull = "Maria Montessori";
    scholarList[9].dates = "(??? - ???)";
    scholarList[9].factSheet = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce non ligula quis arcu porttitor facilisis et et leo. Nullam euismod erat ipsum, in ultrices sapien bibendum sed. Vivamus ac suscipit purus. Aliquam ut viverra felis, sit amet vehicula erat. Curabitur consectetur, nunc vel convallis aliquam, dui turpis euismod est, sed fringilla metus urna id ligula. Maecenas ipsum tortor, commodo a est eget, eleifend sagittis eros. Quisque in sapien aliquet, consequat quam a, convallis metus. Vestibulum tellus ipsum, posuere ut tellus non, lacinia rutrum eros. Mauris ac laoreet urna, non ornare odio.";
    scholarList[9].works = "Food truck neutra you probably haven't heard of them schlitz. Retro viral af fanny pack normcore, twee hella jean shorts art party. Next level polaroid leggings literally. Pop-up distillery slow-carb, waistcoat disrupt chia tilde knausgaard paleo. Four loko whatever paleo pickled vice portland.\nLocavore lumbersexual neutra iPhone pour-over intelligentsia. Fashion axe XOXO vegan snackwave food truck, organic shoreditch. Brooklyn umami hashtag normcore fap. Vexillologist godard polaroid, meggings pinterest copper mug fingerstache chicharrones cronut.";
    
    
    tagList.resize(10);
    tagList[0] = "Arts & Humanities";
    tagList[1] = "Biobehavioral Sciences";
    tagList[2] = "Counseling &  Clinical Psychology";
    tagList[3] = "Curriculum & Teaching";
    tagList[4] = "Education Policy & Social Analysis";
    tagList[5] = "Health and Behavior Studies";
    tagList[6] = "Human Development";
    tagList[7] = "International & Transcultural Studies";
    tagList[8] = "Mathematics, Science & Technology";
    tagList[9] = "Organization & Leadership";

    formattedTagList.resize(tagList.size());
    
    formattedTagList[0].resize(2);
    formattedTagList[0][0] = "Arts &";
    formattedTagList[0][1] = "Humanities";
    
    formattedTagList[1].resize(2);
    formattedTagList[1][0] = "Biobehavioral";
    formattedTagList[1][1] = "Sciences";
    
    formattedTagList[2].resize(2);
    formattedTagList[2][0] = "Counseling &";
    formattedTagList[2][1] = "Clinical Psych.";
    
    formattedTagList[3].resize(2);
    formattedTagList[3][0] = "Curriculum";
    formattedTagList[3][1] = "& Teaching";
    
    formattedTagList[4].resize(2);
    formattedTagList[4][0] = "Educ. Policy";
    formattedTagList[4][1] = "& Soc. Analysis";
    
    formattedTagList[5].resize(2);
    formattedTagList[5][0] = "Health &";
    formattedTagList[5][1] = "Behavior Stud.";
    
    formattedTagList[6].resize(2);
    formattedTagList[6][0] = "Human";
    formattedTagList[6][1] = "Development";
    
    formattedTagList[7].resize(2);
    formattedTagList[7][0] = "Int'l. &";
    formattedTagList[7][1] = "Transcult. Stud.";
    
    formattedTagList[8].resize(2);
    formattedTagList[8][0] = "Math, Science";
    formattedTagList[8][1] = "& Technology";
    
    formattedTagList[9].resize(2);
    formattedTagList[9][0] = "Organization";
    formattedTagList[9][1] = "& Leadership";

    
    tagDescriptions.resize(tagList.size());
    
    tagDescriptions[0] = "This tag... Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean pretium ligula ex, in suscipit felis ornare id. Maecenas sagittis dui orci, nec ";
    tagDescriptions[1] = "This tag... Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean pretium ligula ex, in suscipit felis ornare id. Maecenas sagittis dui orci, nec ";
    tagDescriptions[2] = "This tag... Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean pretium ligula ex, in suscipit felis ornare id. Maecenas sagittis dui orci, nec ";
    tagDescriptions[3] = "This tag... Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean pretium ligula ex, in suscipit felis ornare id. Maecenas sagittis dui orci, nec ";
    tagDescriptions[4] = "This tag... Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean pretium ligula ex, in suscipit felis ornare id. Maecenas sagittis dui orci, nec ";
    tagDescriptions[5] = "This tag... Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean pretium ligula ex, in suscipit felis ornare id. Maecenas sagittis dui orci, nec ";
    tagDescriptions[6] = "This tag... Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean pretium ligula ex, in suscipit felis ornare id. Maecenas sagittis dui orci, nec ";
    tagDescriptions[7] = "This tag... Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean pretium ligula ex, in suscipit felis ornare id. Maecenas sagittis dui orci, nec ";
    tagDescriptions[8] = "This tag... Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean pretium ligula ex, in suscipit felis ornare id. Maecenas sagittis dui orci, nec ";
    tagDescriptions[9] = "This tag... Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean pretium ligula ex, in suscipit felis ornare id. Maecenas sagittis dui orci, nec ";
    
    
    //Go through the tags and set colors
    for(int i = 0; i < tagList.size(); i++){
        
        int hueDiff = 255 / tagList.size();
        
        ofColor tagCol;
        tagCol.setHsb( i * hueDiff, 165, 255);
        
//        scholarList[i].tagCol = tagCol;
        
        //store their tags in a smaller vector for convenience
        tagColorList.push_back(tagCol);
        
    }
    
    
}

//formats long string into a string with line breaks
string ScholarData::formatText(string input, ofTrueTypeFont *font, int maxLineLength){
    
    string output = "";
    
    //split the whole string into a vector of words
    vector<string> words = ofSplitString(input, " ");
    
    int currentLine = 0;

    int spaceLeftInLine = maxLineLength;
    
    
    for(int i = 0; i < words.size(); i++){
        
        //check if the number of characters left in the current line
        //is greater than or equal to the current word, add it
        float wordLength = font -> stringWidth(words[i]);
        
        if(wordLength <= spaceLeftInLine){
            
            //add the word
            output += words[i];
            spaceLeftInLine -= wordLength;
            
            //if there's no more room on the line...
        } else {
            
            //go to next line
            output += "\n";
            currentLine++;
            spaceLeftInLine = maxLineLength;
            
            //add the word
            output += words[i];
            spaceLeftInLine -= wordLength;
            
            

            
        }
        
        //add a space
        output += " ";
        spaceLeftInLine -= font -> stringWidth(" ");
        
        //if thats the end of this line
        //go to the next line
        if(spaceLeftInLine <= 0){
            output += "\n";
            currentLine++;
            spaceLeftInLine = maxLineLength;
        }
        
    }
    
    return output;
    
}



