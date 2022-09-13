import "../../styles/sidepanels/container.scss";

interface Props {
  header: string;
  st?: React.CSSProperties | undefined;
  children: JSX.Element | JSX.Element[];
}

const Container = ({ header, st, children }: Props) => {
  return (
    <div className="container" style={st}>
      <div className="header">
        <p className="name">{header}</p>
      </div>
      {children}
    </div>
  );
};

export default Container;
