import { invoke } from "@tauri-apps/api";
import { useCallback, useEffect, useState } from "react";
import { useSelector } from "react-redux";
import { State } from "../../state/reducers";
import CloseIcon from "@mui/icons-material/Close";
import ArrowDropDownIcon from "@mui/icons-material/ArrowDropDown";
import ArrowDropUpIcon from "@mui/icons-material/ArrowDropUp";
import "../../styles/sidepanels/components.scss";

interface field {
  name: string;
  value: object;
}

interface ComponentProps {
  name: string;
  fields: { [key: string]: object };
}
interface ObjectProps {
  obj: object;
}

const RenderObject = ({ obj }: ObjectProps) => {
  const [val, setVal] = useState<unknown>(obj);
  return (
    <div className="object">
      {(Object.prototype.toString.call(obj) === "[object Array]" &&
        (val as object[]).map((key, index) => <RenderObject obj={key} />)) ||
        (typeof obj === "object" &&
          Object.keys(val as { [key: string]: object }).map((k, i) => (
            <div>
              <p>{k}</p>
              <RenderObject obj={val as { [key: string]: object }} />
            </div>
          ))) ||
        (typeof obj === "number" && (
          <input
            type="number"
            value={val as number}
            onChange={(e) => setVal(e.target.value)}
          />
        )) ||
        (typeof obj === "string" && (
          <input
            type="string"
            value={val as string}
            onChange={(e) => setVal(e.target.value)}
          />
        ))}
    </div>
  );
};

const Field = ({ name, value }: field) => {
  return (
    <div className="field">
      <p>{name}</p>
      <div className="values">
        <RenderObject obj={value as object} />
      </div>
    </div>
  );
};

const Component = ({ name, fields }: ComponentProps) => {
  const [expanded, setExpanded] = useState(false);
  const onExpand = useCallback(() => setExpanded(!expanded), [expanded]);

  return (
    <div className="component">
      <div className="component-name" onClick={onExpand}>
        {expanded ? (
          <ArrowDropUpIcon className="drop-icon" />
        ) : (
          <ArrowDropDownIcon className="drop-icon" />
        )}
        <p>{name}</p>
        <CloseIcon className="close-icon" />
      </div>
      <div
        className={expanded ? "component-values" : "component-values--hidden"}
      >
        {Object.keys(fields).map((key, index) => (
          <Field name={key} value={fields[key]} />
        ))}
      </div>
    </div>
  );
};

const Components = () => {
  const state = useSelector((state: State) => state.entity);
  const [components, setComponents] = useState<
    { [name: string]: { [key: string]: object } } | undefined
  >(undefined);

  useEffect(() => {
    invoke("get_entity_components", { entityid: state })
      .then((value) =>
        setComponents(value as { [name: string]: { [key: string]: object } })
      )
      .catch((err) => console.log(err));
  }, [state]);

  return (
    <div className="components">
      <div className="components-container">
        {components &&
          Object.keys(components).map((key, index) => (
            <Component name={key} fields={components[key]} />
          ))}
      </div>
    </div>
  );
};

export default Components;
