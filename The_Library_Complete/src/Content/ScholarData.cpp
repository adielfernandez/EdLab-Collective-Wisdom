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

    scholarList[0].nameTop = "Mary S.";
    scholarList[0].nameBottom = "Rose";
    scholarList[0].nameFull = "Mary S. Rose";
    scholarList[0].dates = "(1874 - 1941)";
    scholarList[0].factSheet = "Professor Mary Swartz Rose (October 31, 1874 – February 1, 1941) was a Professor of Household Arts from 1910 to 1923 and a Professor of Nutrition from 1923 to 1940 at Teachers College. She co-created the program in Nutrition at TC with Henry Sherman, a Professor of Chemistry at Columbia University over ninety years ago. In doing so, Rose became the first full-time person to develop a program in nutrition at an American university.\n\nRose conducted extensive research on nutrition and dietetics. She designed the first nutrition laboratory devoted solely to training students in this field. In that laboratory, Rose trained others in her life's work. Some of her studies involved bringing this knowledge to the elementary schools.";
    
    scholarList[0].works = "1909: Professor Rose and Henry Sherman, a Professor of Chemistry at Columbia University, created the program in Nutrition at Teachers College.\n1909: Rose became the first full-time person to develop a program in nutrition at an American university.\n1912: 4th ed. 1937: Laboratory Handbook for Dietetics\n1916: Feeding the family\n1918: Everyday foods in wartime\n1929: Foundations of nutrition (Mary Swartz Rose, Grace MacLeod, Clara Mae Taylor)\n1930: The relation of diet to health and growth of children in institutions, with a method of evaluating dietaries and a three-weeks dietary conforming to the standards proposed (Mary Swartz Rose, Cora Emeline Gray, Katherine Lois Foster)";

    
    scholarList[1].nameTop = "Patty Smith";
    scholarList[1].nameBottom = "Hill";
    scholarList[1].nameFull = "Mary S. Rose";
    scholarList[1].dates = "(1868 - 1946)";
    scholarList[1].factSheet = "Patty Smith Hill (March 27, 1868 – May 25, 1946) was an American nursery school, kindergarten teacher, and key founder of the National Association Nursery Education (NANE) which now exists as the National Association For the Education of Young Children (NAEYC). Perhaps most well known as the sister of Mildred J. Hill with whom she is credited as co-writing the tune to the song Good Morning To All which became popular as Happy Birthday to You, Patty developed the Patty Hill blocks and in 1924 helped create the Institute of Child Welfare Research at Columbia University Teachers College.";
    scholarList[1].works = "1892: Hill was a founding member of the International Kindergarten Union (IKU).\n1893: Hill is perhaps best known as the sister of Mildred J. Hill, with whom she is credited as co-writing the tune to the song \"Good Morning To All\". The tune became even more popular as \"Happy Birthday to You\" during the 20th century. Hill and her sister Mildred wrote the song (Mildred wrote the tune; Patty wrote the original lyrics) was honored, along with the Hill sisters, at the Chicago World's Fair in.\n1924: Helped create the Institute of Child Welfare Research.\n1935, Hill became involved with the Federal Emergency Nursery Schools Program.";
    
    
    scholarList[2].nameTop = "William Heard";
    scholarList[2].nameBottom = "Kilpatrick";
    scholarList[2].nameFull = "William Heard Kilpatrick";
    scholarList[2].dates = "(1871 - 1965)";
    scholarList[2].factSheet = "Dr. William Heard Kilpatrick (November 20, 1871 – February 13, 1965) was Professor Emeritus of Philosophy and Education at Teachers College where he taught from 1912 to 1937. As a highly effective proponent of progressive education, advocating project-based learning, curriculum learning, and whole child education, Kilpatrick is considered one of the most popular professors ever at Teachers College, having taught more than 35,000 students during his tenure.\nKilpatrick\'s immensely popular article \"The Project Method\" (1918) made him well known among educators throughout the United States. This approach focuses on the interests of children, which advocates that by using their interests as units of study, learning becomes more relevant and meaningful. His most prominent book Foundations of Method (1925) became a widely used textbook in education courses nationwide.";
    scholarList[2].works = "1918: \"The Project Method\" in the Teachers College Record. Project Method for early childhood education, which was a form of Progressive Education that organized curriculum and classroom activities around a subject's central theme.\n1923: A Source Book in the Philosophy of Education\n1926: Education for a Changing Civilization\n1932: Education and the Social Crisis\n1936: Remaking the Curriculum\n1941: Selfhood and Civilization\n1951:Philosophy of Education";
    
    
    scholarList[3].nameTop = "Edward";
    scholarList[3].nameBottom = "Thorndike";
    scholarList[3].nameFull = "Edward Thorndik";
    scholarList[3].dates = "(1874 - 1949)";
    scholarList[3].factSheet = "Edward “Lee” Thorndike (August 31, 1874 – August 9, 1949) was a psychologist who spent nearly his entire career at Teachers College, His work on Comparative psychology and the learning process led to the theory of connectionism and helped lay the scientific foundation for modern educational psychology. He also worked on solving industrial problems, such as employee exams and testing.";
    scholarList[3].works = "1898: Animal Intelligence, a series of \"puzzlebox\" experiments\n1903: Educational Psychology\n1904: Introduction to Theory of Mental and Social Measurement\n1905: Development of law of effect identifying the three main areas of intellectual development: abstract intelligence, mechanical intelligence, and social intelligence.\n1913–1914 Educational Psychology";
    
    
    
    scholarList[4].nameTop = "John";
    scholarList[4].nameBottom = "Dewey";
    scholarList[4].nameFull = "John Dewey";
    scholarList[4].dates = "(1859 - 1952)";
    scholarList[4].factSheet = "John Dewey (October 20, 1859 – June 1, 1952) was an American philosopher, psychologist, and educational reformer whose ideas have been influential in education and social reform. Dewey is one of the primary figures associated with the philosophy of pragmatism and is considered one of the founders of functional psychology. A well-known public intellectual, he was also a major voice of progressive education and liberalism. Although Dewey is known best for his publications about education, he also wrote about many other topics, including epistemology, metaphysics, aesthetics, art, logic, social theory, and ethics. He was a major educational reformer for the 20th century. The theme of Dewey's works was his profound belief in democracy, be it in politics, education or communication and journalism.";
    scholarList[4].works = "1896: The Reflex Arc Concept in Psychology\n1916: Democracy and Education\n1922:  Human Nature and Conduct\n1927: The Public and its Problems\n1934: Art as Experience\n1904 Became a professor of philosophy at Columbia University while also working at Teachers College";
    
    
    
    scholarList[5].nameTop = "Jerome";
    scholarList[5].nameBottom = "Bruner";
    scholarList[5].nameFull = "Jerome Seymour Bruner";
    scholarList[5].dates = "(1915 - 2016)";
    scholarList[5].factSheet = "Jerome Seymour Bruner (October 1, 1915 – June 5, 2016) was an American psychologist who made significant contributions to human cognitive psychology and cognitive learning theory in educational psychology. Bruner was a senior research fellow at the New York University School of Law.";
    scholarList[5].works = "1960s: the \"Cognitive Revolution\" that continues to influence psychology around the world.\n1961: The Process of Education";
    
    
    
    scholarList[6].nameTop = "Maria";
    scholarList[6].nameBottom = "Montessori";
    scholarList[6].nameFull = "Maria Montessori";
    scholarList[6].dates = "(1870 - 1952)";
    scholarList[6].factSheet = "Maria Tecla Artemisia Montessori August 31, 1870 – May 6, 1952) was an Italian physician and educator best known for the philosophy of education that bears her name, and her writing on scientific pedagogy. Her educational method is in use today in some public and private schools throughout the world.";
    scholarList[6].works = "1907: The Casa dei Bambini, or Children’s House open in n Rome\n1914: The Montesorri System Examined";
    
    
    
    scholarList[7].nameTop = "Jean";
    scholarList[7].nameBottom = "Piaget";
    scholarList[7].nameFull = "Jean Piaget";
    scholarList[7].dates = "(1896 - 1980)";
    scholarList[7].factSheet = "Jean Piaget (9 August 1896 – 16 September 1980) was a Swiss clinical psychologist known for his pioneering work in child development. Piaget placed great importance on the education of children. Piaget's theory and research influenced several people. His theory of child development is studied in pre-service education programs. Educators continue to incorporate constructionist-based strategies.";
    scholarList[7].works = "1936: Theory of cognitive development\n1977: The Grasp of Consciousness: Action and concept in the young child [La prise de conscience (1974)]";
    
    
    
    scholarList[8].nameTop = "Lev";
    scholarList[8].nameBottom = "Vygotsky";
    scholarList[8].nameFull = "Lev Vygotsky";
    scholarList[8].dates = "(1896 - 1934)";
    scholarList[8].factSheet = "Lev Semyonovich Vygotsky (November 17 1896 – June 11, 1934) was a Soviet psychologist, the founder of a theory of human cultural and bio-social development commonly referred to as cultural-historical psychology, and leader of the Vygotsky Circle (also referred to as 'Vygotsky-Luria Circle'). Vygotsky's main work was in developmental psychology, and he proposed a theory of the development of higher cognitive functions in children that saw reasoning as emerging through practical activity in a social environment. He identified play, especially for young children, as the leading source of development in terms of emotional, social, physical, language or cognitive development.";
    scholarList[8].works = "1925: Psychology of Art. Social Development Theory and the \"Zone of Proximal Development\". Believed strongly that community plays a central role in the process of \"making meaning.\"";
    
    scholarList[9].nameTop = "Maxine";
    scholarList[9].nameBottom = "Greene";
    scholarList[9].nameFull = "Maxine Greene";
    scholarList[9].dates = "(1917 - 2014)";
    scholarList[9].factSheet = "Maxine Greene (December 23, 1917 – May 29, 2014) was an educational philosopher, author, social activist and teacher who valued experiential learning in its \"entirety\". Greene influenced thousands of educators to bring the vitality of the arts to teachers and children. For Greene, art provided a conduit to meaning-making, a way of making sense of the world. In 1965, she joined the faculty at Teachers College.";
    scholarList[9].works = "1938 - Graduates from Barnard College\n1949 - Receives a Masters degree, New York University\n1955 - Receives Ph.D., Philosophy of Education, New York University\n1965 as the sole woman in the Philosophy of Education Department\n1965: The Public School and the Private Vision: A Search for America in Education and Literature\n1965 - Editor, Teachers College Record; and Associate Professor English, then Professor of Philosophy of Education, Teachers College, Columbia University\n1973: Teacher as Stranger: Educational Philosophy for the Modern Age\n1973: she was one of the signers of the Humanist Manifesto II\n1976: Philosopher-in-Residence at the Lincoln Center Institute for the Arts in Education\n1984: First female President of the American Educational Research Association in 31 years\n1987: President of the Philosophy of Education Society\n1978: Landscapes of Learning\n1988: The Dialectic of Freedom\n2003: Founded the Maxine Greene Foundation for Social Imagination, the Arts & Education\n2012: Founded the Maxine Greene Center for Aesthetic Education and Social Imagination";
    
    
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



